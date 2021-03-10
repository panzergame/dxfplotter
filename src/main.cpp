#include <view/mainwindow.h>
#include <model/application.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QFontDatabase>
#include <QDebug>

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

	QCommandLineOption profileOption("p", QCoreApplication::translate("main", "Select profile"),
		QCoreApplication::translate("main", "profile"));
	parser.addOption(profileOption);

	parser.process(qapp);

	Model::Application app;
	View::MainWindow window(app);

	if (parser.isSet(toolOption)) {
		const QString toolName = parser.value(toolOption);
		app.selectToolFromCmd(toolName);
	}

	if (parser.isSet(profileOption)) {
		const QString profileName = parser.value(profileOption);
		app.selectProfileFromCmd(profileName);
	}

	// File loading from command line.
	const QString fileName = parser.positionalArguments().value(0, "");
	app.loadFileFromCmd(fileName);

	qapp.exec();
}
