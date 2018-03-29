/*
 * App.cpp
 *
 *  Created on: 2018年3月29日
 *      Author: xd
 */

#include "App.h"


struct LocXY {
	float x, y;
};
struct ScheduleMsg {
	/*命令类型: command, info*/
	String messageType;
	/*命令描述*/
	String text;
	/*时间戳*/
	String timestamp;
	/*命令来源*/
	String from;
	/*子命令集合*/
	struct SubMessage {
		/*子命令类型*/
		String submessage;
		/*坐标点集合*/
		vector<LocXY> points;
		/*是否追加*/
		bool appending;
		/*是否路径规划*/
		bool isMilestone;
		/*负载设备动作时间*/
		uint32_t serveTime;
	};
	vector<SubMessage> submessages;

	bool parseFromJson(String s) {
		return false;
	}
};
void slam_msg_dispatch(const char *topic, const char *msg) {
	String t = topic;
	String m = msg;
	String topicPrefix = "rw/slam/";
	if (!t.start_with(topicPrefix)) {
		fprintf(stderr, "slam msg dispatch get error topic ：%s\n", topic);
		return;
	}
	t = t.skip(topicPrefix.length());
	StringList ss = t.split("/");
	if (ss.size() != 2) {
		fprintf(stderr, "slam msg dispatch get error topic ：%s\n", topic);
		return;
	}
	if (ss[0] == "single") {
		int index = ss[1].tol(10);
		ScheduleMsg sm;
		if (!sm.parseFromJson(msg)) {
			fprintf(stderr, "slam msg parse error ：topic:%s, msg:%msg\n", topic,
					msg);
			return;
		}
		if (sm.messageType == "command") {
			if (sm.text == "move") {

			} else if (sm.text == "charge") {

			} else {
				fprintf(stderr, "bad text value\n");
				return;
			}
		} else if (sm.messageType == "info") {

		} else {
			fprintf(stderr, "bad messageType:%s\n", sm.messageType);
			return;
		}
	} else if (ss[0] == "group") {

	} else {

	}
}

bool App::connectDevices()
{
	vector<Slam*> slams;
		vector<String> slamIpList;
		vector<uint16_t> slamPortList;
		try {
			for(string s : conf.slamsAddrList) {
				String ss = s;
				StringList sl = ss.split(":");
				if(sl.size()==1) {
					slamIpList.push_back(sl[0]);
					slamPortList.push_back(1445);
				} else if(sl.size()==2) {
					slamIpList.push_back(sl[0]);
					slamPortList.push_back((uint16_t)sl[1].tol());
				} else {
					fprintf(stderr,"error config entry: %s\n",s.c_str());
					return -1;
				}
			}
		}catch(...){
			fprintf(stderr,"get a expception on parse config.\n");
			return -1;
		}
		for(uint i=0;i<slamIpList.size();i++){
			Slam *slam = new Slam(i, slamIpList[i], slamPortList[i]);
			slam->nbConnect(conf.slamConTimeout);
			slams.push_back(slam);
		}
		sleep(conf.slamConTimeout+1000);//mabey all ok
		for(Slam *slam : slams){
			if(slam->isConnect()){

			}
		}
		return false;
}


App::App(int argc, char **argv)
{

}

int App::start()
{


//	try {
//		sdp = SlamwareCorePlatform::connect(argv[1], 1445);
//		std::cout <<"SDK Version: " << sdp.getSDKVersion() << std::endl;
//		std::cout <<"SDP Version: " << sdp.getSDPVersion() << std::endl;
//	} catch(ConnectionTimeOutException& e) {
//		std::cout <<e.what() << std::endl;
//		return 1;
//	} catch(ConnectionFailException& e) {
//		std::cout <<e.what() << std::endl;
//		return 1;
//	}
//	rpos::actions::MoveAction moveAction = sdp.getCurrentAction();
//	moveAction.getStatus()

	char path[PATH_MAX];
	path_get_parent(argv[0], &path);
	std::string conf_path = std::string("") + path + "config.ini";
	if (!conf.Load(conf_path.c_str()))
		conf.Save(conf_path.c_str());
	MqttConnecttion mqtt(conf.host, conf.port, conf.user, conf.password,
			conf.MQTT_QOS, conf.mqtt_extra_param);

	if(!connectDevices())
		return -1;


	mqtt.subscribe(TOPIC_SLAM, &slam_msg_dispatch);
	for (;;)
		;
	mqtt.subscribe_exit(TOPIC_SLAM);
	return 0;

}

