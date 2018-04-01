/*
 * Message.h
 *
 *  Created on: Apr 1, 2018
 *      Author: hou
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <vector>
#include "String.h"
using namespace std;

struct LocXY {
	double x, y;
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
		String submessage;	//moveTo|chargeConnect|serve|ctrl

		vector<LocXY> points;	//for moveTo
		bool appending;
		bool isMilestone;

		uint32_t slot;			//for chargeConnect
		String charge_action;

		uint32_t timeout;		//for serve
		String serve_action;

		String direct;			//for ctrl
	};
	vector<SubMessage> submessages;
};

#endif /* MESSAGE_H_ */
