#include "rwproducer.h"

RWProducer::RWProducer(const QHostAddress& host /*= EXAMPLE_HOST*/, const quint16 port /*= EXAMPLE_PORT*/, QObject* parent /*= NULL*/)
	: QMQTT::Client(host, port, parent)
{

}

RWProducer::~RWProducer()
{
}
