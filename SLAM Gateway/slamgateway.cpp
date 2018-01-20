#include "slamgateway.h"

SLAMGateway::SLAMGateway(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	Init();
}

SLAMGateway::~SLAMGateway()
{
	delete m_pRWServer;
	m_pRWServer = NULL;
}

void SLAMGateway::Init()
{
	m_pRWServer = new RWServer(this);

	Start();
}

void SLAMGateway::Start()
{
	m_pRWServer->Start();
}
