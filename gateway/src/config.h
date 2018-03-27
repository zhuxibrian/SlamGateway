/*
 * config.h
 *
 *  Created on: 2018年3月27日
 *      Author: xd
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "HxConfig.h"

struct Config : HxConfig {
	HX_OPTION_VALUE(std::string,	host,		"127.0.0.1")
	HX_OPTION_VALUE(uint16_t,		port,		61883)
	HX_OPTION_VALUE(std::string,	user,		"user")
	HX_OPTION_VALUE(std::string,	password,	"PASSWORDus")
	HX_OPTION_VALUE(uint32_t,		MQTT_QOS,	2)
	HX_OPTION_VALUE(std::string,	mqtt_extra_param,	"")
	HX_OPTION_END()    // !!! must define HX_OPTION_END() !!!
};


#endif /* CONFIG_H_ */
