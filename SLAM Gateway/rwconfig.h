#pragma once

#include <QObject>
#include <QDomDocument>
#include <QFile>
#include "global.h"

class RWConfig : public QObject
{
	Q_OBJECT

public:
	RWConfig(QObject *parent);
	~RWConfig();

public:
	bool praseXML(QString strConfigXMLPath);

private:
	void praseMqttConfig(QDomNode& node);
	void praseSlamList(QDomNode& node);
	void praseSlamMessages(QDomNode& node);
	void praseCtrlMessages(QDomNode& node);

public:
	MqttConfig m_mqttConfig;
	SlamList m_slamList;
	SlamMessages m_slamMessages;
	CtrlMessages m_ctrlMessages;
	QString m_configVersion;
};
