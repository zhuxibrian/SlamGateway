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
		//mSdp(NULL),
		mSlamSdkThread(NULL)
{

}
#include "rpos/core/pose.h"
#include "rpos/core/robot_platform.h"
#include "rpos/robot_platforms/slamware_core_platform.h"
using namespace rpos::core;
using namespace rpos::robot_platforms;
void Slam::threadSlamSdkFn(Slam *thisp, int timeout){
	Location a; // system::util::debug_out()
	RobotPlatform rp;
	try {
		SlamwareCorePlatform sdp;
	} catch (...){

	}
			//thisp->mIp.c_str(), (int)thisp->mPort, timeout);
	//thisp->mSdp = &sdp;
//	try {
//		SlamwareCorePlatform sdp = SlamwareCorePlatform::connect("sdf", 1445);
//		std::cout << "SDK Version: " << sdp.getSDKVersion() << std::endl;
//		std::cout << "SDP Version: " << sdp.getSDPVersion() << std::endl;
//	} catch (ConnectionTimeOutException& e) {
//		std::cout << e.what() << std::endl;
//		return 1;
//	} catch (ConnectionFailException& e) {
//		std::cout << e.what() << std::endl;
//		return 1;
//	}
//	rpos::actions::MoveAction moveAction = sdp.getCurrentAction();
//	moveAction.getStatus()



}
void Slam::connectSlamSdkNb(int timeout)	//非阻塞链接
{

//	 SlamwareCorePlatform sdp;
//	    try {
//	        sdp = SlamwareCorePlatform::connect("", 1445);
//	        std::cout <<"SDK Version: " << sdp.getSDKVersion() << std::endl;
//	        std::cout <<"SDP Version: " << sdp.getSDPVersion() << std::endl;
//	    } catch(ConnectionTimeOutException& e) {
//	        std::cout <<e.what() << std::endl;
//	    } catch(ConnectionFailException& e) {
//	        std::cout <<e.what() << std::endl;
//	    }
//	    std::cout <<"Connection Successfully" << std::endl;
	//mSlamSdkThread = new thread(threadSlamSdkFn,this,timeout);
}
