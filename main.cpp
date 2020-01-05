#include <view/mainwindow.h>

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QCommandLineParser parser;
	parser.addPositionalArgument("file", "input file");
	parser.process(app);

	const QString fileName = parser.positionalArguments().value(0, "");

	View::MainWindow window(fileName);

	app.exec();
}
