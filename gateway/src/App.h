/*
 * App.h
 *
 *  Created on: 2018年3月29日
 *      Author: xd
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#include <vector>
#include <list>
#include <cstdint>
#include "rpos/rpos.h"
#include "rpos/robot_platforms/slamware_core_platform.h"
#include "rpos/features/location_provider/map.h"
#include "config.h"
#include "utils.h"
#include "mqtt_wrap.h"
#include "String.h"
#include "cJSON.h"
#include "Slam.h"
using namespace std;
using namespace rpos::core;
using namespace rpos::robot_platforms;
using namespace rpos::features;
using namespace rpos::features::location_provider;

#define TOPIC_SLAM		"rw/slam/#"

#ifndef APP_H_
#define APP_H_

class App {
public:
	int argc; char *argv[10];
	Config conf;
	vector<Slam*> slams;
	App(int _argc, char **_argv);
	int start();
	bool connectDevices();
};

#endif /* APP_H_ */
