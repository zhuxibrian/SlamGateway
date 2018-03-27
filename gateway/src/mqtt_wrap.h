/*
 * mqtt_wrap.h
 *
 *  Created on: 2018年3月27日
 *      Author: xd
 */

#ifndef MQTT_WRAP_H_
#define MQTT_WRAP_H_

#include <cstdint>
#include <string>
#include <map>
#include <thread>
using namespace std;
typedef void OnMsgRecvFn(const char *topic, const char *msg);
struct MqttSub {
	string topic;
	FILE *fstream;
	volatile bool runFlag;
	OnMsgRecvFn *on_msg_recv;
	thread *sub_thread;
};
class MqttConnecttion {
private:
	string mHost;
	uint16_t mPort;
	string mUser;
	string mPassword;
	int mQos;
	string mExtraParams;
	map<string,MqttSub*> mSubList;
public:
	MqttConnecttion(const string &host, uint16_t port, const string &user,
			const string &password, int Qos = 2, const string &extra = "");
	virtual ~MqttConnecttion() {
	}
	bool subscribe(const char *topic,
			OnMsgRecvFn *on_msg_recv);
	bool subscribe_exit(const char *topic);
	bool public_msg(const char *topic, const char *msg);
};

#endif /* MQTT_WRAP_H_ */
