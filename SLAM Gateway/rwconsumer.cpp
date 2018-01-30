#include "rwconsumer.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

#define ServerInfo m_pRWServer->ServerInfo

RWConsumer::RWConsumer(const QHostAddress& mqttHost, const quint16 port, RW::SlamConfig slamConfig, RWServer* parent)
	: QMQTT::Client(mqttHost, port, parent),m_slam(NULL),m_timer(new QTimer)
{
	m_slamConfig = slamConfig;
	m_pRWServer = parent;
	connect(this, &RWConsumer::connected, this, &RWConsumer::onConnected);
	connect(this, &RWConsumer::subscribed, this, &RWConsumer::onSubscribed);
	connect(this, &RWConsumer::received, this, &RWConsumer::onReceived);
	connect(this, &RWConsumer::disconnected, this, &RWConsumer::onDisconnected);
}

RWConsumer::~RWConsumer()
{
	if (m_slam)
	{
		m_slam->disconnect();
		delete m_slam;
		m_slam = NULL;
	}
	if (m_timer)
	{
		delete m_timer;
		m_timer = NULL;
	}
	m_pRWServer = NULL;
	disconnectFromHost();
}

void RWConsumer::publishToCtrl(RW::MqttMsgStruct mqttMsg, int submessageIndex)
{
	QJsonObject json;
	json.insert("messageType", mqttMsg.mqttMsgHead.msgType);
	json.insert("from", mqttMsg.mqttMsgHead.msgFrom);
	json.insert("timestamp", QString(mqttMsg.mqttMsgHead.timestamp));

	QVariantMap variantMap = mqttMsg.submessageList[submessageIndex];
	QJsonObject submessage;
	QMapIterator<QString, QVariant> threaditer(variantMap);
	while (threaditer.hasNext())
	{
		submessage.insert(threaditer.next().key(), threaditer.value().toString());
	}
	QJsonArray array;
	array.push_back(submessage);

	json.insert("submessages", array);

	// 构建 JSON 文档
	QJsonDocument document;
	document.setObject(json);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	QString strJson(byteArray);

	QString topic = TOPIC_RW_CTRL_SINGLE + m_slamConfig["id"];
	QMQTT::Message message(mqttMsg.mqttMsgHead.timestamp, topic,
		strJson.toUtf8(), MQTT_QOS_2);
	publish(message);
}

void RWConsumer::onConnected()
{
	ServerInfo("slam:" + m_slamConfig["id"] + " connected mqtt broker");
	subscribe(TOPIC_RW_SLAM_ALL, 0);
	subscribe(TOPIC_RW_SLAM_GROUP + m_slamConfig["group"], 2);
	subscribe(TOPIC_RW_SLAM_SINGLE + m_slamConfig["id"], 2);
	subscribe(TOPIC_RW_SLAM_INFO, 0);
}

void RWConsumer::onDisconnected()
{
	ServerInfo("slam:" + m_slamConfig["id"] + " disconnected mqtt broker");
}

void RWConsumer::onSubscribed(const QString& topic)
{
	ServerInfo("slam:" + m_slamConfig["id"] + " subsrcibed topic:" + topic);
}

void RWConsumer::onReceived(const QMQTT::Message& message)
{
	ServerInfo("slam:" + m_slamConfig["id"] + " receive message.topic:" + message.topic() + " messages:" + QString::fromUtf8(message.payload()));
	RW::MqttMsgStruct mqttMsgStruct;
	praseMessage(message, mqttMsgStruct);
	if (mqttMsgStruct.mqttMsgHead.msgType == "info")
	{
		//一般是rw module发送的状态信息，用于改变rwslam对象状态。
		m_slam->setStatus(mqttMsgStruct);
	}
	else
	{
		m_mqttMsgList.push_back(mqttMsgStruct);
	}
}

/************************************************************************/
/* 连接mqtt broker														*/
/* 连接slam module，连接失败，每隔10秒自动重新连接						*/
/************************************************************************/
void RWConsumer::onStart()
{
	connectToHost();
	onConnectSlam();
	if (!m_slam->isConnect())
	{
		QTimer::connect(m_timer, SIGNAL(timeout()), this, SLOT(onConnectSlam()));
		m_timer->start(10000);
	}
}

/************************************************************************/
/* 连接slam module                                                                     */
/************************************************************************/
void RWConsumer::onConnectSlam()
{
	if (m_slam == NULL)
		m_slam = new RWZero(this);
	
	if (!m_slam->isConnect())
	{
		try
		{
			m_slam->connect(m_slamConfig["ip"], m_slamConfig["port"].toInt());
			ServerInfo("slam:" + m_slamConfig["id"] + " connect slam module");
			QTimer::disconnect(m_timer, SIGNAL(timeout()), this, SLOT(onConnectSlam()));
			QTimer::connect(m_timer, SIGNAL(timeout()), this, SLOT(checkSlamStatus()));
			m_timer->start(1000);
		}
		catch (QString e)
		{
			ServerInfo("slam:" + m_slamConfig["id"] + " " + e);
		}
	}
	else
		m_timer->stop();
}

bool RWConsumer::praseMessage(const QMQTT::Message& message, RW::MqttMsgStruct& mqttMsgStruct)
{
	if (!m_slam->isConnect())
		return false;

	QString strMessage = QString::fromUtf8(message.payload());
	
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(strMessage.toLocal8Bit().data(), &jsonError);
	if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
	{
		if (document.isObject())
		{
			QJsonObject object = document.object();
			praseMessageHead(object, mqttMsgStruct);
			praseMessageSubmessages(object, mqttMsgStruct);
		}
	}
	return true;

}

void RWConsumer::praseMessageHead(QJsonObject& object, RW::MqttMsgStruct& mqttMsgStruct)
{
	if (object.contains("messageType"))
	{
		QJsonValue value = object.value("messageType");
		if (value.isString())
			mqttMsgStruct.mqttMsgHead.msgType = value.toString();
	}
	if (object.contains("timestamp"))
	{
		QJsonValue value = object.value("timestamp");
		if (value.isString())
			mqttMsgStruct.mqttMsgHead.timestamp = value.toInt();
	}
	if (object.contains("from"))
	{
		QJsonValue value = object.value("from");
		if (value.isString())
			mqttMsgStruct.mqttMsgHead.msgFrom = value.toString();
	}
}

void RWConsumer::praseMessageSubmessages(QJsonObject& object, RW::MqttMsgStruct& mqttMsgStruct)
{
	if (object.contains("submessages"))
	{
		QVariantList submessages = object.value("submessages").toVariant().toList();
		for each (QVariant var in submessages)
		{
			QVariantMap submessage = var.toMap();
			mqttMsgStruct.submessageList.push_back(submessage);
		}
	}
}

void RWConsumer::checkMessage(RW::MqttMsgStruct mqttMsgStruct)
{
	if (mqttMsgStruct.submessageList.size() == 0)
		return;

	if (mqttMsgStruct.mqttMsgHead.msgType == "")
	{
	}
}

void RWConsumer::checkSlamStatus()
{
	bool isIdle = m_slam->isIdle();
	if (isIdle && m_mqttMsgList.size()>0)
	{
		RW::MqttMsgStruct mqttMsgStruct = m_mqttMsgList[0];
		m_slam->setCommand(mqttMsgStruct);
		m_mqttMsgList.pop_front();
		m_slam->dowork();
	}

	QJsonObject json;
	QString strIdle = isIdle ? "1" : "0";
	json.insert("idle", strIdle);
	m_slam->getSlamInfo(json);
	publishToSch(json);
}

void RWConsumer::publishToSch(QJsonObject &jsonobject)
{
	jsonobject.insert("messageType", "slamInfo");
	jsonobject.insert("from", TOPIC_RW_SLAM_SINGLE + m_slamConfig["id"]);

	QDateTime time = QDateTime::currentDateTime();
	int timeT = time.toTime_t();
	jsonobject.insert("timestamp", QString::number(timeT));

	// 构建 JSON 文档
	QJsonDocument document;
	document.setObject(jsonobject);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	QString strJson(byteArray);

	QMQTT::Message message(timeT, TOPIC_RW_SCH, strJson.toUtf8(), MQTT_QOS_0);
	publish(message);
}

