#pragma once

#include <QCoreApplication>
#include <QMessageBox>
#include <QMap>
#include <QVector>

typedef QMap<QString, QString> MqttConfig;
typedef QMap<QString, QString> SlamConfig;
typedef QVector<SlamConfig> SlamList;
typedef QVector<QString> SlamMessages;
typedef QVector<QString> CtrlMessages;

#define CONFIGURATIONPATH QCoreApplication::applicationDirPath()+"/config.xml"
