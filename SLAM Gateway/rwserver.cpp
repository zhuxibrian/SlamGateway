#include "rwserver.h"
#include "rwconfig.h"

RWServer::RWServer(QObject *parent)
	: QObject(parent)
{
}

RWServer::~RWServer()
{
}

void RWServer::Start()
{
	RWConfig rwConfig(this);
	rwConfig.praseXML(CONFIGURATIONPATH);
}