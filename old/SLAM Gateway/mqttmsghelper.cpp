#include "mqttmsghelper.h"
#include <QVariant>

MqttMsgHelper::MqttMsgHelper()
{
}

MqttMsgHelper::~MqttMsgHelper()
{
}

QString MqttMsgHelper::convertToMqttMsg(QJsonObject jsonobject, QString strID)
{
	jsonobject.insert("messageType", "slamInfo");
	jsonobject.insert("from", TOPIC_RW_SLAM_SINGLE + strID);

	jsonobject.insert("timestamp", QString::number(getTimestamp()));

	// 构建 JSON 文档
	QJsonDocument document;
	document.setObject(jsonobject);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	QString strJson(byteArray);
	return strJson;
}

QString MqttMsgHelper::convertToMqttMsg(RW::MqttMsgStruct mqttMsg, int submessageIndex)
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

	return strJson;
}

quint16 MqttMsgHelper::getTimestamp()
{
	QDateTime time = QDateTime::currentDateTime();
	int timeT = time.toTime_t();
	return timeT;
}


