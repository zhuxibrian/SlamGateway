#pragma once

#include <QCoreApplication>
#include <QMessageBox>
#include <QMap>
#include <QVector>
#include "rwexception.h"

typedef QMap<QString, QString> MqttConfig;
typedef QMap<QString, QString> SlamConfig;
typedef QVector<SlamConfig> SlamList;
typedef QVector<QString> SlamMessages;
typedef QVector<QString> CtrlMessages;


#define SERVERINFOMAXCOUNT 4096
#define CONFIGURATIONPATH QCoreApplication::applicationDirPath()+"/config.xml"

//¶©ÔÄÖ÷Ìâ
#define TOPIC_RW_SLAM_ALL		"rw/slam/all"
#define TOPIC_RW_SLAM_GROUP		"rw/slam/group/"
#define TOPIC_RW_SLAM_SINGLE	"rw/slam/single/"
#define TOPIC_RW_SLAM_INFO		"rw/slam/info/"
