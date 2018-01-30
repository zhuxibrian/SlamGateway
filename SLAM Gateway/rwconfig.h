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
	//void praseSlamMessages(QDomNode& node);
	//void praseCtrlMessages(QDomNode& node);

public:
	RW::MqttConfig m_MqttConfig;
	RW::SlamList m_SlamList;
	//RW::SlamMessages m_SlamMessages;
	//RW::CtrlMessages m_CtrlMessages;
	QString m_configVersion;
};
