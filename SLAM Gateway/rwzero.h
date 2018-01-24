#pragma once

#include "rwslam.h"

class RWZero : public RWSlam
{
	Q_OBJECT

public:
	RWZero(QObject* parent);
	~RWZero();

private:
	//rpos::robot_platforms::SlamwareCorePlatform platform;
};
