#include <control/application.h>

#include <QStandardPaths>
#include <QDebug>
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
	QApplication qapp(argc, argv);
	qapp.setApplicationName("dxfplotter");

	Control::Application app(argv[1]);

	app.exportToGcode("");
}
