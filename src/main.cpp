#include <ui_mainwindow.h>
#include <view/mainwindow.h>
#include <model/application.h>

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
	QApplication qapp(argc, argv);
	qapp.setApplicationName("dxfplotter");
	qapp.setApplicationDisplayName("dxfplotter");

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addPositionalArgument("file", "input file");
	QCommandLineOption toolOption("t", QCoreApplication::translate("main", "Select tool"),
		QCoreApplication::translate("main", "tool"));
	parser.addOption(toolOption);
	parser.process(qapp);

	Model::Application app;
	View::MainWindow window(app);

	if (parser.isSet(toolOption)) {
		const QString toolName = parser.value(toolOption);
		app.selectToolFromCmd(toolName);
	}

	// File loading from command line.
	const QString fileName = parser.positionalArguments().value(0, "");
	app.loadFileFromCmd(fileName);

	qapp.exec();
}
