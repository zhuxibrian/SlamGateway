#pragma once

#include <QObject>

class RWConsumer : public QObject
{
	Q_OBJECT

public:
	RWConsumer(QObject *parent);
	~RWConsumer();
};
