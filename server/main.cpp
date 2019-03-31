#include "myserver.h"
#include <QCoreApplication>

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	MyServer server;
	server.StartServer();

	return a.exec();
}
