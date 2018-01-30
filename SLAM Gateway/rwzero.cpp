#include "rwzero.h"
#include <QJsonObject>
#include <QJsonArray>

RWZero::RWZero(QObject* parent)
	: RWSlam()
{
	m_parent = (RWConsumer *)parent;
}

RWZero::~RWZero()
{
}

void RWZero::connect(QString ip, int port)
{
	try
	{
		m_platform = SlamwareCorePlatform::connect(ip.toStdString(), port);
		m_isConnected = true;
	}
	catch (ConnectionFailException & /*xcp*/)
	{
		throw QString("Connect SDP failed!");
	}
	catch (std::exception & xcp)
	{
		throw QString("Connect SDP failed!");
	}
}

bool RWZero::isConnect()
{
	return m_isConnected;
}

void RWZero::disconnect()
{
	m_isConnected = false;
	m_platform.disconnect();
}

void RWZero::setStatus(RW::MqttMsgStruct mqttMsgStruct)
{
	//改变状态
}

void RWZero::setCommand(RW::MqttMsgStruct mqttMsgStruct)
{
	m_currentMqttMsg = mqttMsgStruct;
	m_currentSubmessageIndex = 0;
}

void RWZero::dowork()
{
	RW::Submessage submsg = m_currentMqttMsg.submessageList[m_currentSubmessageIndex];
	switch (RW::mapSlamActions[submsg["submessage"].toString()])
	{
	case RW::SlamAction::forward:
		forward();
		break;
	case RW::SlamAction::backward:
		backward();
		break;
	case RW::SlamAction::leftward:
		leftward();
		break;
	case RW::SlamAction::rightward:
		rightward();
		break;
	case RW::SlamAction::moveTo:
		moveTo(submsg);
		break;
	default:
		m_parent->publishToCtrl(m_currentMqttMsg, m_currentSubmessageIndex);
	}
	m_currentSubmessageIndex++;
}

bool RWZero::isIdle()
{
	//获得当前状态
	rpos::actions::MoveAction curAction = m_platform.getCurrentAction();
	if (!curAction.isEmpty())
		return false;

	//先判断当前是否有未完成的子命令
	if (m_currentMqttMsg.submessageList.size() > m_currentSubmessageIndex)
	{
		dowork();
		return false;
	}

	return true;
}

void RWZero::getSlamInfo(QJsonObject& jsonObject)
{
	rpos::core::Location location = m_platform.getLocation();
	jsonObject.insert("locationX", QString::number(location.x(), 'f', 3));
	jsonObject.insert("locationY", QString::number(location.y(), 'f', 3));
}

void RWZero::forward()
{
	rpos::core::Direction direction(rpos::core::ACTION_DIRECTION::FORWARD);
	m_platform.moveBy(direction);
}

void RWZero::backward()
{
	rpos::core::Direction direction(rpos::core::ACTION_DIRECTION::BACKWARD);
	m_platform.moveBy(direction);
}

void RWZero::leftward()
{
	rpos::core::Direction direction(rpos::core::ACTION_DIRECTION::TURNLEFT);
	m_platform.moveBy(direction);
}

void RWZero::rightward()
{
	rpos::core::Direction direction(rpos::core::ACTION_DIRECTION::TURNRIGHT);
	m_platform.moveBy(direction);
}

void RWZero::moveTo(const RW::Submessage submessage)
{
	QVariantList points;
	if (submessage["submessage"] == "moveTo" && submessage.contains("points"))
		points = submessage.value("points").toList();

	QVector<rpos::core::Location> locations;
	for each (QVariant value in points)
	{
		QVariantMap point = value.toMap();
		double x, y, z;
		x = point["x"].toDouble(0);
		y = point["y"].toDouble(0);
		z = point["z"].toDouble(0);

		rpos::core::Location loc(x, y, z);
		locations.push_back(loc);
	}

	bool appending = submessage["appending"].toString() == "1" ? true : false;
	bool isMilestone = submessage["isMilestone"].toString() == "1" ? true : false;

	if (locations.size() == 1)
		rpos::actions::MoveAction moveAction = m_platform.moveTo(locations[0], appending, isMilestone);
	else if (locations.size() > 1)
	{
		rpos::actions::MoveAction moveAction = m_platform.moveTo(locations.toStdVector(), appending, isMilestone);
	}

	//		rpos::core::Location loc(location.x, location.y, location.z);

	//rpos::actions::MoveAction moveAction = m_platform.moveTo(loc, appending, isMilestone);
}
