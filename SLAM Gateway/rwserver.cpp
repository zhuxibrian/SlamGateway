#include "rwserver.h"
#include "rwconfig.h"



RWServer::RWServer(QObject *parent)
	: QObject(parent)
{
}

RWServer::~RWServer()
{
	Stop();
	ServerInfo("RW Server Stop.");
}

void RWServer::Start()
{
	ServerInfo("RW Server Start.");
	RWConfig rwConfig(this);
	rwConfig.praseXML(CONFIGURATIONPATH);
	ServerInfo("RW Server Prase xml config.");

	
	QHostAddress host(rwConfig.m_mqttConfig["ip"]);
	qint16 port = rwConfig.m_mqttConfig["port"].toInt();
	if (host.isNull() || port == 0)
		throw RWException("123", 123, "mqtt broker ip/port is invaild");
	

	for (int i=0; i<rwConfig.m_slamList.size(); i++)
	{
		SlamConfig slamConfig = rwConfig.m_slamList[i];
		if (slamConfig["ip"] == "" || slamConfig["port"] == "")
			continue;

		RWConsumer *rwConsumer = new RWConsumer(host, port, slamConfig, this);
		QThread *thread = new QThread(this);
		connect(this, SIGNAL(startConsumers()), rwConsumer, SLOT(onStart()));
		rwConsumer->moveToThread(thread);
		m_rwConsumerMap[slamConfig["id"].toInt()] = rwConsumer;
		m_rwThreadMap[slamConfig["id"].toInt()] = thread;
		thread->start();
	}

	emit startConsumers();

}

void RWServer::Stop()
{
	QMapIterator<int, QThread*> threaditer(m_rwThreadMap);
	while (threaditer.hasNext())
	{
		if (threaditer.next().value())
		{
			if (threaditer.value()->isRunning())
			{
				threaditer.value()->quit();
				threaditer.value()->wait();
			}
			delete m_rwThreadMap[threaditer.key()];
			m_rwThreadMap[threaditer.key()] = NULL;
		}
	}
	m_rwThreadMap.clear();

	QMapIterator<int, RWConsumer*> consumeriter(m_rwConsumerMap);
	while (consumeriter.hasNext())
	{
		int key = consumeriter.next().key();
		if (m_rwConsumerMap[key])
		{
			disconnect(this, SIGNAL(startConsumers()), m_rwConsumerMap[key], SLOT(onStart));
			delete m_rwConsumerMap[key];
			m_rwConsumerMap[key] = NULL;
		}

	}
	m_rwConsumerMap.clear();
}

void RWServer::ServerInfo(QString info)
{
	emit serverInfo(info);
}
