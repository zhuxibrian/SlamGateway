#pragma once

#include <QObject>
#include "rwconsumer.h"
#include "global.h"
#include <QThread>

class RWConsumer;
typedef QMap<int, RWConsumer*> RWConsumerMap;
typedef QMap<int, QThread*> RWThreadMap;

class RWServer : public QObject
{
	Q_OBJECT

public:
	RWServer(QObject *parent);
	~RWServer();

signals:
	void serverInfo(QString);
	void startConsumers();

public:
	void Start();
	void Stop();
	void ServerInfo(QString info);

private:
	RWConsumerMap m_rwConsumerMap;
	RWThreadMap m_rwThreadMap;
	
};
