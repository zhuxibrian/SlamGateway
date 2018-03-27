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
#include "rpos/rpos.h"
#include "config.h"
#include "utils.h"
#include "mqtt_wrap.h"
#include "String.h"
using namespace std;
using namespace rpos::core;

#define TOPIC_SLAM		"rw/slam/#"

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

		return;
	}
	if(ss[0]=="single"){
		int index = ss[1].tol(10);
		//...
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
	return 0;
}
