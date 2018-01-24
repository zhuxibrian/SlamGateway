#include "rwconsumer.h"

#define ServerInfo m_pRWServer->ServerInfo

RWConsumer::RWConsumer(const QHostAddress& mqttHost, const quint16 port, SlamConfig slamConfig, RWServer* parent)
	: QMQTT::Client(mqttHost, port, parent)
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
	m_pRWServer = NULL;
	disconnectFromHost();
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
}

void RWConsumer::onStart()
{
	connectToHost();
}

