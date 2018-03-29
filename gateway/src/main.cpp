//============================================================================
//Name:slam-gateway.cpp
//Author:houxd
//Version:
//Copyright:copyrightruiwei
//Description:HelloWorldinC++,Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#include <vector>
#include <cstdint>
#include "rpos/rpos.h"
#include "rpos/robot_platforms/slamware_core_platform.h"
#include "rpos/features/location_provider/map.h"
#include "config.h"
#include "utils.h"
#include "mqtt_wrap.h"
#include "String.h"
#include "cJSON.h"
using namespace std;
using namespace rpos::core;
using namespace rpos::robot_platforms;
using namespace rpos::features;
using namespace rpos::features::location_provider;

#define TOPIC_SLAM		"rw/slam/#"

struct LocXY {
	float x,y;
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

	bool parseFromJson(String s){
		return false;
	}
};
void slam_msg_dispatch(const char *topic, const char *msg) {
	String t = topic;
	String m = msg;
	String topicPrefix = "rw/slam/";
	if(!t.start_with(topicPrefix)){
		fprintf(stderr,"slam msg dispatch get error topic ：%s\n",topic);
		return;
	}
	t = t.skip(topicPrefix.length());
	StringList ss = t.split("/");
	if(ss.size()!=2){
		fprintf(stderr,"slam msg dispatch get error topic ：%s\n",topic);
		return;
	}
	if(ss[0]=="single"){
		int index = ss[1].tol(10);
		ScheduleMsg sm;
		if(!sm.parseFromJson(msg)){
			fprintf(stderr,"slam msg parse error ：topic:%s, msg:%msg\n",topic,msg);
			return;
		}
		if(sm.messageType == "command"){
			if(sm.text == "move"){

			}else if(sm.text == "charge"){

			}else{
				fprintf(stderr,"bad text value\n");
				return;
			}
		}else if(sm.messageType == "info"){

		}else{
			fprintf(stderr,"bad messageType:%s\n",sm.messageType);
			return;
		}
	}else if(ss[0]=="group"){

	}else{


	}
}
int main(int argc, char **argv) {
	Config conf;
	char path[PATH_MAX];
	path_get_parent(argv[0], &path);
	std::string conf_path = std::string("") + path + "config.ini";
	if (!conf.Load(conf_path.c_str()))
		conf.Save(conf_path.c_str());
	MqttConnecttion mqtt(conf.host, conf.port, conf.user, conf.password,
			conf.MQTT_QOS, conf.mqtt_extra_param);
	mqtt.subscribe(TOPIC_SLAM, &slam_msg_dispatch);
	for (;;)
		;
	mqtt.subscribe_exit(TOPIC_SLAM);


	SlamwareCorePlatform sdp;
	    try {
	        sdp = SlamwareCorePlatform::connect(argv[1], 1445);
	        std::cout <<"SDK Version: " << sdp.getSDKVersion() << std::endl;
	        std::cout <<"SDP Version: " << sdp.getSDPVersion() << std::endl;
	    } catch(ConnectionTimeOutException& e) {
	        std::cout <<e.what() << std::endl;
	        return 1;
	    } catch(ConnectionFailException& e) {
	        std::cout <<e.what() << std::endl;
	        return 1;
	    }
	    rpos::actions::MoveAction moveAction = sdp.getCurrentAction();
	    moveAction.getStatus()
	return 0;

}
