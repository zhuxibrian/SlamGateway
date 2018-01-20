#pragma once

#include <QObject>
#include <qmqtt.h>

const QHostAddress EXAMPLE_HOST = QHostAddress::LocalHost;
const quint16 EXAMPLE_PORT = 1883;

class RWProducer : public QMQTT::Client
{
	Q_OBJECT

public:
	explicit RWProducer(const QHostAddress& host = EXAMPLE_HOST,
						const quint16 port = EXAMPLE_PORT,
						QObject* parent = NULL);
	~RWProducer();


public:
	
	

};
