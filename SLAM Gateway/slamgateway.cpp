#include "slamgateway.h"
#include "global.h"
#include <QScrollBar>

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
	connect(m_pRWServer, SIGNAL(serverInfo(QString)), this, SLOT(onServerInfo(QString)));

	Start();
}

void SLAMGateway::Start()
{
	try
	{
		m_pRWServer->Start();
	}
	catch (RWException* e)
	{
		onServerInfo(e->getError());
	}
	
}

void SLAMGateway::onServerInfo(QString info)
{
	if (m_strServerInfo.length() > SERVERINFOMAXCOUNT)
		m_strServerInfo.clear();
	m_strServerInfo += info;
	m_strServerInfo += "\r\n";
	ui.serverInfo->setText(m_strServerInfo);
	QScrollBar *scrollbar = ui.serverInfo->verticalScrollBar();
	if (scrollbar)
	{
		scrollbar->setSliderPosition(scrollbar->maximum());
	}
}

