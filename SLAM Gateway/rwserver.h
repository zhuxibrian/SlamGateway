#pragma once

#include <QObject>

class RWServer : public QObject
{
	Q_OBJECT

public:
	RWServer(QObject *parent);
	~RWServer();

	void Start();

	
};
