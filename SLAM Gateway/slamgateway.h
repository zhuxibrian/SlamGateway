#pragma once

#include <QtWidgets/QMainWindow>
#include "rwserver.h"
#include "ui_slamgateway.h"

class SLAMGateway : public QMainWindow
{
	Q_OBJECT

public:
	SLAMGateway(QWidget *parent = Q_NULLPTR);
	~SLAMGateway();

	
public:
	void Init();
	void Start();

public slots :
	void onServerInfo(QString info);

private:
	RWServer *m_pRWServer;

	QString m_strServerInfo;

private:
	Ui::SLAMGatewayClass ui;

};
