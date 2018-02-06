#pragma once

#include <QObject>
#include "global.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

class MqttMsgHelper
{
public:
	MqttMsgHelper();
	~MqttMsgHelper();


	QString convertToMqttMsg(RW::MqttMsgStruct mqttMsg,int submessageIndex);
	QString convertToMqttMsg(QJsonObject jsonobject, QString strID);

	quint16 getTimestamp();
};
