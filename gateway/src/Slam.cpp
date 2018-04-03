/*
 * Slam.cpp
 *
 *  Created on: 2018年3月29日
 *      Author: xd
 */

#include "Slam.h"
#include "main.h"
Slam::Slam(int index, String ip, uint16_t port) :
		mSlamId(index), mIp(ip), mPort(port), mCtrlBoardIp(""), mSlamSdkConnected(false),
		mCtrlBoardConnected(false),
		mSdp(),
		mSlamSdkThread(NULL),
		msgIndex4Ctrl(0),
		mSdpMutex()
{
		 text = "";
		 location = {0,0};
		 serveState = "";
		 faultInfo = "";
		 rotation = {0,0,0};

		 battery = 0;
		 state = "idle";
}

void Slam::threadSlamSdkFn(Slam *thisp, int timeout){
	try {
		thisp->mSdp = SlamwareCorePlatform::connect(thisp->mIp,(int)thisp->mPort,timeout);
		//std::cout << "SDK Version: " << thisp->mSdp.getSDKVersion() << std::endl;
		//std::cout << "SDP Version: " << thisp->mSdp.getSDPVersion() << std::endl;
		thisp->updateState();
		thisp->setSlamSdkConnected(true);
	} catch (ConnectionTimeOutException& e) {
		vfault("error when connect slam sdk %s\n",e.what());
		thisp->state = "fault";
		thisp->faultInfo = String("connect ") + e.what();
	} catch (ConnectionFailException& e) {
		vfault("error when connect slam sdk %s\n",e.what());
		thisp->state = "fault";
		thisp->faultInfo = String("connect ") + e.what();
	}
}
void Slam::connectSlamSdkNb(int timeout)	//非阻塞链接
{
	mSlamSdkThread = new thread(threadSlamSdkFn,this,timeout);
}
void Slam::waitLastMoveAction()
{
	mSdpMutex.lock();
	state = "busy";
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
	mSdpMutex.lock();
	state = "idle";
	mSdpMutex.unlock();
}
void Slam::performTask(MqttConnecttion& mqtt,
		vector<ScheduleMsg::SubMessage> task) {
	for(ScheduleMsg::SubMessage sm : task){
		if(sm.submessage == "moveTo"){
			waitLastMoveAction();
			vector<Location> locs;
			for(LocXY l : sm.points){
				locs.push_back(Location(l.x,l.y));
			}
			mSdpMutex.lock();
			mSdp.moveTo(locs, sm.appending, sm.isMilestone);
			mSdpMutex.unlock();
		}else if(sm.submessage == "ctrl"){
			//stop current move
			mSdpMutex.lock();
			rpos::actions::MoveAction moveAction = mSdp.getCurrentAction();
			moveAction.cancel();
			mSdpMutex.unlock();
			//do ctrl
			if(sm.direct=="stop"){
				//is stoped.
			}else if(sm.direct == "forward"){
				mSdpMutex.lock();
				mSdp.moveBy(ACTION_DIRECTION::FORWARD);
				mSdpMutex.unlock();
			}else if(sm.direct == "backward"){
				mSdpMutex.lock();
				mSdp.moveBy(ACTION_DIRECTION::BACKWARD);
				mSdpMutex.unlock();
			}else if(sm.direct == "turnright"){
				mSdpMutex.lock();
				mSdp.moveBy(ACTION_DIRECTION::TURNRIGHT);
				mSdpMutex.unlock();
			}else if(sm.direct == "turnleft"){
				mSdpMutex.lock();
				mSdp.moveBy(ACTION_DIRECTION::TURNLEFT);
				mSdpMutex.unlock();
			}else{
				fprintf(stderr,"error direct : %s\n",sm.direct.c_str());

			}
		}else{
			String topic = String().snprintf(512,"rw/ctrl/single/",mSlamId);
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
}
void Slam::updateState()
{
	mSdpMutex.lock();
	Pose pose = mSdp.getPose();
	rotation = pose.rotation();
	state = mSdp.getBatteryIsCharging()?"charging":state;
	location = mSdp.getLocation();

	mSdpMutex.unlock();
}

