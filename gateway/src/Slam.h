/*
 * Slam.h
 *
 *  Created on: 2018年3月29日
 *      Author: xd
 */

#ifndef SLAM_H_
#define SLAM_H_

#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>
#include "Message.h"
#include "mqtt_wrap.h"
#include "String.h"
#include "rpos/rpos.h"
#include "rpos/core/pose.h"
#include "rpos/core/robot_platform.h"
#include "rpos/robot_platforms/slamware_core_platform.h"
using namespace rpos::core;
using namespace rpos::robot_platforms;
using namespace std;
class Slam {
	int mSlamId;
	String mIp;
	uint16_t mPort;
	String mCtrlBoardIp;
	atomic<bool> mSlamSdkConnected;
	atomic<bool> mCtrlBoardConnected;
	SlamwareCorePlatform mSdp;
	static void threadSlamSdkFn(Slam *thisp, int timeout);
	thread *mSlamSdkThread;
	uint64_t msgIndex4Ctrl;
	mutex mSdpMutex;
public:
	String text;
	Location location;
	String serveState;
	String faultInfo;
	Rotation rotation;

	uint32_t battery;
	String state;

	Slam(int index, String ip, uint16_t port);
	void connectSlamSdkNb(int timeout);	//非阻塞链接
	void performTask(MqttConnecttion &mqtt, vector<ScheduleMsg::SubMessage> task);
	void waitLastMoveAction();
	void updateState();
	int getSlamId() const {
		return mSlamId;
	}

	const String& getIp() const {
		return mIp;
	}

	uint16_t getPort() const {
		return mPort;
	}

	const String& getCtrlBoardIp() const {
		return mCtrlBoardIp;
	}

	void setCtrlBoardIp(const String& ctrlBoardIp) {
		mCtrlBoardIp = ctrlBoardIp;
	}

	bool isCtrlBoardConnected() const {
		return mCtrlBoardConnected;
	}

	void setCtrlBoardConnected(bool alive = false) {
		mCtrlBoardConnected = alive;
	}

	bool isSlamSdkConnected() const {
		return mSlamSdkConnected;
	}

	void setSlamSdkConnected(bool slamSdkConnected = false) {
		mSlamSdkConnected = slamSdkConnected;
	}
};

#endif /* SLAM_H_ */
