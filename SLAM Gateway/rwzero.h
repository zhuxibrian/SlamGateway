#pragma once

#include "rwslam.h"
#include "global.h"
#include <rpos/robot_platforms/slamware_core_platform.h>
#include "rwconsumer.h"

class RWConsumer;

using namespace rpos::robot_platforms;
using namespace rpos::features;
using namespace rpos::features::location_provider;

class RWZero : public RWSlam
{
	Q_OBJECT

public:
	RWZero(QObject* parent);
	~RWZero();

	virtual void connect(QString ip, int port);
	virtual bool isConnect();
	virtual void disconnect();
	virtual void setStatus(RW::MqttMsgStruct mqttMsgStruct);
	virtual void setCommand(RW::MqttMsgStruct mqttMsgStruct);
	virtual void dowork();
	virtual bool isIdle();

	virtual void getSlamInfo(QJsonObject& jsonObject);

protected:
	virtual void forward();
	virtual void backward();
	virtual void leftward();
	virtual void rightward();
	virtual void moveTo(const RW::Submessage submessage);

private:
	SlamwareCorePlatform m_platform;	
	RWConsumer *m_parent;
};
