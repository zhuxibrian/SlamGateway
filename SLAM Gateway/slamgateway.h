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

private:
	RWServer *m_pRWServer;

private:
	Ui::SLAMGatewayClass ui;

};
