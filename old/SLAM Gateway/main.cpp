#include "slamgateway.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SLAMGateway w;
	w.show();
	return a.exec();
}
