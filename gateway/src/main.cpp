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
#include <list>
#include <cstdint>
#include "config.h"
#include "utils.h"
#include "mqtt_wrap.h"
#include "String.h"
#include "cJSON.h"
#include "Slam.h"
using namespace std;


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

static Config conf;
static vector<Slam*> slams;
MqttConnecttion *mqtt;
int msgIndex4Ctrl=0;

static void slam_msg_dispatch(const char *topic, const char *msg, void *vp) {
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
			fprintf(stderr, "slam msg parse error ：topic:%s, msg:%s\n", topic,
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
			fprintf(stderr, "bad messageType:%s\n", sm.messageType.c_str());
			return;
		}
	} else if (ss[0] == "group") {

	} else {

	}
}
static void on_ctrl_single_msg(const char *topic, const char *msg, void *vp)
{
	String t = topic;
	Slam *slam = (Slam*)vp;
	if(t.start_with("rw/ctrl/single/")){
		int index;
		if(sscanf(topic,"rw/ctrl/single/%d",&index)!=1)
			goto err;
		if(slam->getIndex()!=index){
			fprintf(stderr,"error index :want %d get %d\n",slam->getIndex(),index);
			goto err;
		}
		cJSON *json = cJSON_Parse(msg);
		String messageType = cJSON_Print(cJSON_GetObjectItem(json,"messageType"));
		String timestamp = cJSON_Print(cJSON_GetObjectItem(json,"timestamp"));
		String msgIndex = cJSON_Print(cJSON_GetObjectItem(json,"msgIndex"));
		String result = cJSON_Print(cJSON_GetObjectItem(json,"result"));
		String faultInfo = cJSON_Print(cJSON_GetObjectItem(json,"faultInfo"));
		if(messageType=="chargeConnect"){

		}if(messageType=="serve"){

		}if(messageType=="getState"){
			slam->setCtrlBoardIp(result);
			slam->setCtrlBoardConnected(true);
		}
	}
	return;
err:
	fprintf(stderr,"error:%s get %s\n",__FUNCTION__,topic);
}
static bool connectDevices()
{
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
		slam->connectSlamSdkNb(conf.slamConTimeout);
		slams.push_back(slam);
	}
	sleep(conf.slamConTimeout+1000);//mabey all ok
	cJSON *msg = cJSON_CreateObject();
	cJSON_AddItemToObject(msg,"messageType",cJSON_CreateString("slamInfo"));
	cJSON_AddItemToObject(msg,"timestamp",cJSON_CreateNumber(time(NULL)));
	cJSON_AddItemToObject(msg,"msgIndex",cJSON_CreateNumber(msgIndex4Ctrl));
	cJSON *k_message = cJSON_CreateArray();
	for(Slam *slam : slams){
		if(slam->isSlamSdkConnected()){
			cJSON *item = cJSON_CreateObject();
			cJSON_AddItemToObject(item,"index",cJSON_CreateNumber(slam->getIndex()));
			cJSON_AddItemToObject(item,"slamIp",cJSON_CreateString(slam->getIp().c_str()));
			cJSON_AddItemToObject(item,"slamPort",cJSON_CreateNumber(slam->getPort()));
			cJSON_AddItemToArray(k_message,item);
			//subscribe target ctrl, check connect.
			mqtt->subscribe(String().snprintf(512,"/rw/ctrl/single/%d",slam->getIndex()),
					on_ctrl_single_msg, slam);
		}else{
			fprintf(stderr,"the target slam sdk cannot connect ip:%s index:%d\n",
					slam->getIp().c_str(),slam->getPort());
		}
	}
	//boardcast for all ctrls, try to connect them.
	cJSON_AddItemToObject(msg,"message",k_message);
	mqtt->public_msg("rw/ctrl/all",cJSON_Print(msg));
	cJSON_Delete(msg);
	bool allConnected = true;
	for(int i=0;i<(conf.slamCtrlBoardConTimeout/500);i++){
		sleep(500);
		for(Slam *slam : slams){
			if(slam->isSlamSdkConnected() && (!slam->isCtrlBoardConnected()))
				allConnected = false;
		}
		if(allConnected)
			break;
	}
	if(!allConnected)
		printf("Not all slam sdk have ctrlboard connected!!!\n");
	//ok, work for connect complete.
	return true;
}

int main(int argc, char **argv) {
	cJSON_Hooks cjsonHooks = {malloc,free};
	cJSON_InitHooks(&cjsonHooks);
	char path[PATH_MAX];
	path_get_parent(argv[0], &path);
	std::string conf_path = std::string("") + path + "config.ini";
	if (!conf.Load(conf_path.c_str()))
		conf.Save(conf_path.c_str());
	mqtt = new MqttConnecttion(conf.host, conf.port, conf.user, conf.password,
			conf.MQTT_QOS, conf.mqtt_extra_param);
	if (!connectDevices())
		return -1;

	mqtt->subscribe("rw/slam/#", &slam_msg_dispatch);
	for (;;);
	mqtt->subscribe_exit("rw/slam/#");
	return 0;

}



