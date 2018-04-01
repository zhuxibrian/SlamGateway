/*
 * Slam.cpp
 *
 *  Created on: 2018年3月29日
 *      Author: xd
 */

#include "Slam.h"
Slam::Slam(int index, String ip, uint16_t port) :
		mIndex(index), mIp(ip), mPort(port), mCtrlBoardIp(""), mSlamSdkConnected(false),
		mCtrlBoardConnected(false),
		mSdp(),
		mSlamSdkThread(NULL),
		msgIndex4Ctrl(0),
		mPerformMutex(),
		mSdpMutex()
{
		 text = "";
		 location = {0,0};
		 serveState = "";
		 faultInfo = "";
		 rotation = {0,0,0};

		 battery = 0;
		 state = "";
}

void Slam::threadSlamSdkFn(Slam *thisp, int timeout){
	try {
		thisp->mSdp = SlamwareCorePlatform::connect(thisp->mIp,(int)thisp->mPort,timeout);
		//std::cout << "SDK Version: " << thisp->mSdp.getSDKVersion() << std::endl;
		//std::cout << "SDP Version: " << thisp->mSdp.getSDPVersion() << std::endl;
		thisp->setSlamSdkConnected(true);
	} catch (ConnectionTimeOutException& e) {
		std::cout << "error when connect slam sdk " << e.what() << std::endl;
	} catch (ConnectionFailException& e) {
		std::cout << "error when connect slam sdk " << e.what() << std::endl;
	}
}
void Slam::connectSlamSdkNb(int timeout)	//非阻塞链接
{
	mSlamSdkThread = new thread(threadSlamSdkFn,this,timeout);
}

void Slam::performTask(MqttConnecttion& mqtt,
		vector<ScheduleMsg::SubMessage> task) {
	mPerformMutex.lock();
	for(ScheduleMsg::SubMessage sm : task){
		if(sm.submessage == "moveTo"){
			mSdpMutex.lock();
			rpos::actions::MoveAction moveAction = mSdp.getCurrentAction();
			mSdpMutex.unlock();
			if(moveAction){
				for(;;){
					usleep(1000000);
					mSdpMutex.lock();
					ActionStatus as = moveAction.getStatus();
					mSdpMutex.unlock();
					if(as==rpos::core::ActionStatusFinished)
						break;
				}
			}
			vector<Location> locs;
			for(LocXY l : sm.points){
				locs.push_back(Location(l.x,l.y));
			}
			mSdpMutex.lock();
			moveAction = mSdp.moveTo(locs, sm.appending, sm.isMilestone);
			mSdpMutex.unlock();
		}else if(sm.submessage == "ctrl"){
#warning ...
		}else{
			String topic = String().snprintf(512,"rw/ctrl/single/",mIndex);
			String msg;
			if(sm.submessage == "chargeConnect"){
				msg = String().snprintf(4096,
					"{"
					"	\"messageType\":\"chargeConnect\","
					"	\"timestamp\":%u,"
					"	\"msgIndex\": %llu,"
					"	\"message\":{"
					"		\"slot\":%u"
					"		\"action\":\"%s\""
					"	}"
					"}",
					(uint32_t)time(NULL),
					msgIndex4Ctrl,
					sm.slot,
					sm.charge_action.c_str()
					);
				mqtt.public_msg(topic.c_str(),msg.c_str());
			}else if(sm.submessage == "serve"){
				msg = String().snprintf(4096,
					"{"
					"	\"messageType\":\"chargeConnect\","
					"	\"timestamp\":%u,"
					"	\"msgIndex\": %llu,"
					"	\"message\":{"
					"		\"timeout\":%u"
					"		\"action\":\"%s\""
					"	}"
					"}",
					(uint32_t)time(NULL),
					msgIndex4Ctrl,
					sm.timeout,
					sm.serve_action.c_str());
				mqtt.public_msg(topic.c_str(),msg.c_str());
			}else{
				fprintf(stderr,"unknown submessage form sch\n");
			}
		}
	}
	mPerformMutex.unlock();
}

