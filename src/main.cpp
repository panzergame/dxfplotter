#include <view/mainwindow.h>
#include <model/application.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QFontDatabase>
#include <QFile>
#include <QDebug>

QString readStyleSheet()
{
	QFile file(":stylesheet.qss");
	file.open(QFile::ReadOnly | QFile::Text);
	return file.readAll();
}

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(resource);

	QApplication qapp(argc, argv);
	qapp.setApplicationName("dxfplotter");
	qapp.setApplicationDisplayName("dxfplotter");
	qapp.setStyleSheet(readStyleSheet());

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

	model::Application app;
	view::MainWindow window(app);

	if (parser.isSet(toolOption)) {
		const QString toolName = parser.value(toolOption);
		app.defaultToolFromCmd(toolName);
	}

	if (parser.isSet(profileOption)) {
		const QString profileName = parser.value(profileOption);
		app.defaultProfileFromCmd(profileName);
	}

	// File loading from command line.
	const QString fileName = parser.positionalArguments().value(0, "");
	app.loadFileFromCmd(fileName);

	qapp.exec();
}
