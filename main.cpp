#include <view/mainwindow.h>
#include <core/application.h>

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
	QApplication qapp(argc, argv);

	QCommandLineParser parser;
	parser.addPositionalArgument("file", "input file");
	parser.process(qapp);

	const QString fileName = parser.positionalArguments().value(0, "");

	Core::Application app(fileName);
	View::MainWindow window(app);

	qapp.exec();
}
