#include <ui_mainwindow.h>
#include <view/mainwindow.h>
#include <control/application.h>

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
	QApplication qapp(argc, argv);

	QCommandLineParser parser;
	parser.addPositionalArgument("file", "input file");
	parser.process(qapp);

	const QString fileName = parser.positionalArguments().value(0, "");

	Control::Application app(fileName);

	View::MainWindow window(app);

	qapp.exec();
}
