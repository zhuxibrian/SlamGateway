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
		mSlamSdkThread(NULL)
{

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
	while(1)sleep(10000);
}
void Slam::connectSlamSdkNb(int timeout)	//非阻塞链接
{
	mSlamSdkThread = new thread(threadSlamSdkFn,this,timeout);
}
