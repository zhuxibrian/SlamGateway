#pragma once

#include <QCoreApplication>
#include <QMessageBox>
#include <QMap>
#include <QVector>
#include <QTimer>
#include <QDateTime>

#define SERVERINFOMAXCOUNT 4096
#define CONFIGURATIONPATH QCoreApplication::applicationDirPath()+"/config.xml"

	//订阅主题
#define TOPIC_RW_SLAM_ALL		"rw/slam/all"
#define TOPIC_RW_SLAM_GROUP		"rw/slam/group/"
#define TOPIC_RW_SLAM_SINGLE	"rw/slam/single/"
#define TOPIC_RW_SLAM_INFO		"rw/slam/info/"

#define TOPIC_RW_CTRL_SINGLE	"rw/ctrl/single/"

#define TOPIC_RW_SCH			"rw/sch"

#define MQTT_QOS_0		0
#define MQTT_QOS_1		1
#define MQTT_QOS_2		2


namespace RW
{
	struct MqttMsgHead
	{
		QString msgType;
		QString msgFrom;
		int timestamp;
	};

	typedef QVariantMap Submessage;
	typedef QVector<Submessage> SubmessageList;
	struct MqttMsgStruct
	{
		MqttMsgHead mqttMsgHead;		//msg头信息
		SubmessageList submessageList;	//子命令列表
	};

	typedef QMap<QString, QString> MqttConfig;
	typedef QMap<QString, QString> SlamConfig;
	typedef QVector<SlamConfig> SlamList;
	typedef QVector<QString> SlamMessages;
	typedef QVector<QString> CtrlMessages;

	static enum SlamAction {
		forward,
		backward,
		leftward,
		rightward,
		moveTo
	};
	static QMap<QString, SlamAction> mapSlamActions;
}

