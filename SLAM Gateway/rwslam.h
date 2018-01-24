#pragma once

#include <QObject>
#include <qmqtt.h>

class RWSlam : public QObject
{
	Q_OBJECT

public:
	RWSlam(QObject* parent);
	~RWSlam();

public:
	virtual void connect() = 0;
	virtual void move(double locationX, double locationY, double locationZ) = 0;
};
