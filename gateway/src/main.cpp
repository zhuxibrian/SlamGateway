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
#include <cstdlib>
#include "config.h"
#include "utils.h"
#include "mqtt_wrap.h"
#include "String.h"
#include "cJSON.h"
#include "Slam.h"
#include "Message.h"
#include "main.h"
using namespace std;

static Config conf;
static vector<Slam*> slams;
static MqttConnecttion *mqtt;
static int verboseLevel = 0;

int verbose(int v, const char *fmt, ...)
{
	int res;
	if(v<verboseLevel){
		va_list va;
		va_start(va,fmt);
		res = vprintf(fmt,va);
		va_end(va);
	}
	return res;
}
static bool parseSchedualMsg(const String &json, ScheduleMsg &msg)
{

	cJSON *j = cJSON_Parse(json.c_str());
	msg.messageType  = cJSON_Print(cJSON_GetObjectItem(j, "messageType"));
	msg.timestamp = cJSON_Print(cJSON_GetObjectItem(j, "timestamp"));
	msg.text = cJSON_Print(cJSON_GetObjectItem(j, "text"));
	msg.from = cJSON_Print(cJSON_GetObjectItem(j, "from"));
	cJSON *submessages = cJSON_GetObjectItem(j, "submessages");
	int sz = cJSON_GetArraySize(submessages);
	msg.submessages.clear();
	for(int i=0;i<sz;i++){
		cJSON *message = cJSON_GetArrayItem(submessages,i);
		ScheduleMsg::SubMessage sm;
		sm.submessage = cJSON_Print(cJSON_GetObjectItem(message, "submessage"));
		cJSON *points = cJSON_GetObjectItem(message,"point");
		int pointsArraySz = cJSON_GetArraySize(points);
		sm.points.clear();
		for(int j=0;j<pointsArraySz;j++){
			cJSON *pt = cJSON_GetArrayItem(points,i);
			double x = String(cJSON_Print(cJSON_GetObjectItem(pt,"x"))).tod();
			double y = String(cJSON_Print(cJSON_GetObjectItem(pt,"y"))).tod();
			sm.points.push_back(LocXY(x,y));
		}
		sm.appending = String(cJSON_Print(cJSON_GetObjectItem(message, "appending"))) == "1";
		sm.isMilestone = String(cJSON_Print(cJSON_GetObjectItem(message, "appending"))) == "1";
		sm.slot = String(cJSON_Print(cJSON_GetObjectItem(message, "slot"))).toul();
		sm.charge_action = cJSON_Print(cJSON_GetObjectItem(message, "action"));
		sm.timeout = String(cJSON_Print(cJSON_GetObjectItem(message, "charge_action"))).toul();
		sm.serve_action = cJSON_Print(cJSON_GetObjectItem(message, "action"));
		sm.direct = cJSON_Print(cJSON_GetObjectItem(message, "direct"));
		msg.submessages.push_back(sm);
	}

	return false;
}
static void on_schedule_msg(const char *topic, const char *msg, void *vp) {
	String t = topic;
	String m = msg;
	String topicPrefix = "rw/slam/";
	if (!t.start_with(topicPrefix)) {
		vfault( "slam msg dispatch get error topic ：%s\n", topic);
		return;
	}
	t = t.skip(topicPrefix.length());
	StringList ss = t.split("/");
	if (ss.size() != 2) {
		vfault( "slam msg dispatch get error topic ：%s\n", topic);
		return;
	}
	if (ss[0] == "single") {
		int index = ss[1].tol(10);
		Slam *slam = slams[index];
		if((!slam->isSlamSdkConnected()) || (!slam->isCtrlBoardConnected())){
			vfault("error:  target slam or ctrl board not connect .\n");
			return;
		}
		ScheduleMsg sm;
		if (!parseSchedualMsg(String(msg),sm)) {
			vfault( "slam msg parse error ：topic:%s, msg:%s\n", topic,
					msg);
			return;
		}
		//check timestamp msgindex .
		//...

		if (sm.messageType == "command") {
			slam->performTask(*mqtt,sm.submessages);
		} else {
			vfault( "bad messageType:%s\n", sm.messageType.c_str());
			return;
		}
	} else if (ss[0] == "group") {

	} else {

	}
}
static void on_ctrl_msg(const char *topic, const char *msg, void *vp)
{
	String t = topic;
	Slam *slam = (Slam*)vp;
	if(t.start_with("rw/ctrl/single/")){
		int index;
		if(sscanf(topic,"rw/ctrl/single/%d",&index)!=1)
			goto err;
		if(slam->getSlamId()!=index){
			vfault("error index :want %d get %d\n",slam->getSlamId(),index);
			goto err;
		}
		cJSON *json = cJSON_Parse(msg);
		String messageType = cJSON_Print(cJSON_GetObjectItem(json,"messageType"));
		String timestamp = cJSON_Print(cJSON_GetObjectItem(json,"timestamp"));
		String ip = cJSON_Print(cJSON_GetObjectItem(json,"ip"));
		String state = cJSON_Print(cJSON_GetObjectItem(json,"state"));
		String battery = cJSON_Print(cJSON_GetObjectItem(json,"battery"));
		String serveState = cJSON_Print(cJSON_GetObjectItem(json,"serveState"));
		String faultInfo = cJSON_Print(cJSON_GetObjectItem(json,"faultInfo"));
		if(messageType!="info")
			goto err;
		if(ip!="")
			slam->setCtrlBoardIp(ip);
		slam->setCtrlBoardConnected(true);
		slam->battery = battery.tol(10);
		slam->state = state;
		slam->serveState = serveState;
		slam->faultInfo= faultInfo;
	}
	return;
err:
	vfault("error:%s get %s\n",__FUNCTION__,topic);
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
				vfault("error config entry: %s\n",s.c_str());
				return -1;
			}
		}
	}catch(...){
		vfault("get a expception on parse config.\n");
		return -1;
	}
	for(uint i=0;i<slamIpList.size();i++){
		Slam *slam = new Slam(i, slamIpList[i], slamPortList[i]);
#ifdef __DEBUG__
		slam->setSlamSdkConnected();
#else
		slam->connectSlamSdkNb(conf.slamConTimeout);
#endif
		slams.push_back(slam);
	}
	usleep((conf.slamConTimeout+1000)*1000);//mabey all ok

	cJSON *msg = cJSON_CreateObject();
	cJSON_AddItemToObject(msg,"messageType",cJSON_CreateString("slamInfo"));
	cJSON_AddItemToObject(msg,"timestamp",cJSON_CreateNumber(time(NULL)));
	cJSON_AddItemToObject(msg,"msgIndex",cJSON_CreateNumber(0/*boardcast allways 0*/));
	cJSON *k_message = cJSON_CreateArray();
	for(Slam *slam : slams){
		if(slam->isSlamSdkConnected()){
			cJSON *item = cJSON_CreateObject();
			cJSON_AddItemToObject(item,"slamId",cJSON_CreateNumber(slam->getSlamId()));
			cJSON_AddItemToObject(item,"slamIp",cJSON_CreateString(slam->getIp().c_str()));
			cJSON_AddItemToObject(item,"slamPort",cJSON_CreateNumber(slam->getPort()));
			cJSON_AddItemToArray(k_message,item);
			//subscribe target ctrl, check connect.
			mqtt->subscribe(String().snprintf(512,"/rw/ctrl/single/%d",slam->getSlamId()),
					on_ctrl_msg, slam);
		}else{
			vfault("the target slam sdk cannot connect ip:%s index:%d\n",
					slam->getIp().c_str(),slam->getPort());
		}
	}
	//boardcast for all ctrls, try to connect them.
	cJSON_AddItemToObject(msg,"message",k_message);
	mqtt->public_msg("rw/ctrl/all",cJSON_Print(msg));
	cJSON_Delete(msg);

	bool allConnected = true;
	for(uint i=0;i<(conf.slamCtrlBoardConTimeout/500);i++){
		usleep(500000);
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
	printf(	"Slam Gateway Program V0.1\n");
	printf(	"------------------------------------------------------------\n");
	if(argc>=2){
		verboseLevel = String(argv[1]).toul();
	}
	cJSON_Hooks cjsonHooks = {malloc,free};
	cJSON_InitHooks(&cjsonHooks);
	char path[PATH_MAX];
	path_get_parent(argv[0], &path);
	std::string conf_path = std::string(path) + "config.ini";
	if (!conf.Load(conf_path.c_str()))
		conf.Save(conf_path.c_str());
	mqtt = new MqttConnecttion(conf.host, conf.port, conf.user, conf.password,
			conf.MQTT_QOS, conf.mqtt_extra_param);
	printf("connect slam sdk & ctrl boards. maybe use about 10s.\n");
	if (!connectDevices())
		return -1;

	mqtt->subscribe("rw/slam/#", &on_schedule_msg);
	for (;;){
		String msg = String().snprintf(4096,
			"{"
			"	\"messageType\":\"info\","
			"	\"timestamp\":%u,"
			"	\"slams\":[",
			time(NULL));
		for(Slam* slam : slams){
#ifndef __DEBUG__
			slam->updateState();
#endif
			msg= msg+String().snprintf(4096,
				"{"
				"	\"slamId\":%u,"
				"	\"text\":\"%s\","
				"	\"state\":\"%s\","
				"	\"location\":\"%lf,%lf\","
				"	\"battery\":%u,"
				"	\"serveState\":\"%s\","
				"	\"faultInfo\":\"%s\","
				"	\"rotation\":\"%lf,%lf,%lf\""
				"},",
				slam->getSlamId(),
				slam->text.c_str(),
				slam->state.c_str(),
				slam->location.x(),slam->location.y(),
				slam->battery,
				slam->serveState.c_str(),
				slam->faultInfo.c_str(),
				slam->rotation.pitch(),slam->rotation.roll(),slam->rotation.yaw()
			);
		}
		msg = msg+
		"	]"
		"}";
		mqtt->public_msg("rw/slam/state",msg);
		usleep(1000000);
	}
	mqtt->subscribe_exit("rw/slam/#");
	return 0;

}


