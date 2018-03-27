#pragma once

#include <QObject>
#include <qmqtt.h>
#include "global.h"

class RWSlam : public QObject
{
	Q_OBJECT

public:
	virtual void connect(QString ip, int port) = 0;
	virtual bool isConnect() = 0;
	virtual void disconnect() = 0;

	virtual void setStatus(RW::MqttMsgStruct mqttMsgStruct) = 0;
	virtual void setCommand(RW::MqttMsgStruct mqttMsgStruct) = 0;
	virtual void dowork() = 0;
	virtual bool isIdle() = 0;
	virtual void getSlamInfo(QJsonObject& jsonObject) = 0;

protected:
	virtual void forward() = 0;
	virtual void backward() = 0;
	virtual void leftward() = 0;
	virtual void rightward() = 0;
	virtual void moveTo(const RW::Submessage submessage) = 0;


	
protected:
	bool m_isConnected;
	RW::MqttMsgStruct m_currentMqttMsg;			//当前正在执行的命令
	int m_currentSubmessageIndex;				//当前正在执行的子命令序号


};
