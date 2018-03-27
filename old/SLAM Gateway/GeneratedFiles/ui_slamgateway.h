/********************************************************************************
** Form generated from reading UI file 'slamgateway.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLAMGATEWAY_H
#define UI_SLAMGATEWAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SLAMGatewayClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTextEdit *serverInfo;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SLAMGatewayClass)
    {
        if (SLAMGatewayClass->objectName().isEmpty())
            SLAMGatewayClass->setObjectName(QStringLiteral("SLAMGatewayClass"));
        SLAMGatewayClass->resize(640, 480);
        SLAMGatewayClass->setMinimumSize(QSize(640, 480));
        SLAMGatewayClass->setMaximumSize(QSize(640, 480));
        centralWidget = new QWidget(SLAMGatewayClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        serverInfo = new QTextEdit(centralWidget);
        serverInfo->setObjectName(QStringLiteral("serverInfo"));

        horizontalLayout->addWidget(serverInfo);

        SLAMGatewayClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SLAMGatewayClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 26));
        SLAMGatewayClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SLAMGatewayClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMinimumSize(QSize(50, 50));
        SLAMGatewayClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SLAMGatewayClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SLAMGatewayClass->setStatusBar(statusBar);

        retranslateUi(SLAMGatewayClass);

        QMetaObject::connectSlotsByName(SLAMGatewayClass);
    } // setupUi

    void retranslateUi(QMainWindow *SLAMGatewayClass)
    {
        SLAMGatewayClass->setWindowTitle(QApplication::translate("SLAMGatewayClass", "SLAMGateway", 0));
    } // retranslateUi

};

namespace Ui {
    class SLAMGatewayClass: public Ui_SLAMGatewayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLAMGATEWAY_H
