/*
 * mqtt_wrap.cpp
 *
 *  Created on: 2018年3月27日
 *      Author: xd
 */

#include "mqtt_wrap.h"
#include "utils.h"
#include "main.h"
#include <cstdio>
#include <cstring>

#define CMD_BUFSIZ		(4096)
#define MQTT_SUB_BUFSIZ	(4096)

MqttConnecttion::MqttConnecttion(const string &host, uint16_t port,
		const string &user, const string &password, int Qos,
		const string &extra) :
		mHost(host), mPort(port), mUser(user), mPassword(password), mQos(Qos), mExtraParams(extra){
	mSubList.clear();
}
static void mqtt_sub_fn(MqttSub *sub, void *vp)
{
	char buf[MQTT_SUB_BUFSIZ];
	while(sub->runFlag && NULL!=fgets(buf,MQTT_SUB_BUFSIZ,sub->fstream)){
		buf[MQTT_SUB_BUFSIZ-1]=0;
		char *p = strchr(buf,' ');
		*p++='\0';
		sub->on_msg_recv(buf,p,vp);
	}
}
bool MqttConnecttion::subscribe(const char *topic,
		OnMsgRecvFn *on_msg_recv, void *vp) {
	vinfo(	"mqtt subscribe: %s\n",topic);
	string t = topic;
	if(mSubList.find(t) != mSubList.end()){
		printf("subscribe:  topic already exist.");
		subscribe_exit(topic);
	}
	char cbuf[CMD_BUFSIZ];
	snprintf(cbuf, CMD_BUFSIZ,
			"mosquitto_sub -h %s -p %hu -u %s -P %s -q %d %s -t \'%s\'", mHost.c_str(),
			mPort, mUser.c_str(), mPassword.c_str(), mQos, mExtraParams.c_str(), topic);
	vnormal("execute:%s\n",cbuf);
	FILE *fs = popen(cbuf, "r");
	if(fs==NULL){
		vfault("mosquitto cmd error: %s\n",cbuf);
		return false;
	}
	MqttSub *sub = new MqttSub;
	sub->topic = t;
	sub->fstream = fs;
	sub->runFlag = true;
	sub->on_msg_recv = on_msg_recv;
	sub->sub_thread = new thread(mqtt_sub_fn,sub,vp);
	mSubList[t] = sub;
	return true;
}
bool MqttConnecttion::subscribe_exit(const char *topic) {
	vinfo(	"mqtt subscribe exit: %s\n",topic);
	string t = topic;
	if(mSubList.find(t) != mSubList.end()){
		MqttSub *sub = mSubList[t];
		pclose(sub->fstream);
		sub->runFlag = false;
		sub->sub_thread->join();
		mSubList.erase(t);
	}
	return true;
}
bool MqttConnecttion::public_msg(const char *topic, const char *msg) {
	vinfo(	"mqtt public:\n"
			"topic: %s\n"
			"msg: %s\n",topic,msg);
	char cbuf[CMD_BUFSIZ];
	snprintf(cbuf, CMD_BUFSIZ,
			"mosquitto_pub -h %s -p %hu -u %s -P %s -q %d %s -t %s -m \'%s\'", mHost.c_str(),
			mPort, mUser.c_str(), mPassword.c_str(), mQos, mExtraParams.c_str(), topic, msg);
	char rbuf[1024];
	vnormal("execute:%s\n",cbuf);
	shell_exec(rbuf,1024,"%s",cbuf);
	return true;
}
