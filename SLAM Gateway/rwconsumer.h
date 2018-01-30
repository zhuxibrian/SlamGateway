#pragma once

#include <QObject>
#include "qmqtt.h"
#include "global.h"
#include "rwserver.h"
#include "rwzero.h"

class RWServer;
class RWConsumer : public QMQTT::Client
{
	Q_OBJECT

public:
	RWConsumer(const QHostAddress& mqttHost, const quint16 port, RW::SlamConfig slamConfig, RWServer* parent = NULL);
	~RWConsumer();

	void publishToCtrl(RW::MqttMsgStruct mqttMsg, int submessageIndex);

public slots:
	void onConnected();
	void onDisconnected();
	void onSubscribed(const QString& topic);
	void onReceived(const QMQTT::Message& message);

	void onStart();
	void onConnectSlam();	

private:
	bool praseMessage(const QMQTT::Message& message, RW::MqttMsgStruct& mqttMsgStruct);
	void praseMessageHead(QJsonObject& object, RW::MqttMsgStruct& mqttMsgStruct);
	void praseMessageSubmessages(QJsonObject& object, RW::MqttMsgStruct& mqttMsgStruct);
	void checkMessage(RW::MqttMsgStruct mqttMsgStruct);
	void checkSlamStatus();
	void publishToSch(QJsonObject &jsonobject);

private:
	RW::SlamConfig m_slamConfig;
	RWServer *m_pRWServer;
	RWSlam *m_slam;

	QVector<RW::MqttMsgStruct> m_mqttMsgList;	//¥˝÷¥––√¸¡Ó¡–±Ì

	QTimer *m_timer;
	

};
