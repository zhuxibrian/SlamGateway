#pragma once

#include <QObject>
#include "qmqtt.h"
#include "global.h"
#include "rwserver.h"

class RWServer;
class RWConsumer : public QMQTT::Client
{
	Q_OBJECT

public:
	RWConsumer(const QHostAddress& mqttHost, const quint16 port, SlamConfig slamConfig, RWServer* parent = NULL);
	~RWConsumer();

public slots:
	void onConnected();
	void onDisconnected();
	void onSubscribed(const QString& topic);
	void onReceived(const QMQTT::Message& message);

	void onStart();

private:
	SlamConfig m_slamConfig;
	RWServer *m_pRWServer;
	

};
