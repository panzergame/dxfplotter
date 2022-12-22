#include <view/mainwindow.h>
#include <model/application.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QFontDatabase>
#include <QFile>
#include <QDebug>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include <QVTKOpenGLNativeWidget.h>

void setDarkPalette(QApplication &qapp)
{
	QPalette palette;
	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Base, QColor(25, 25, 25));
	palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	palette.setColor(QPalette::ToolTipBase, QColor(26, 130, 186));
	palette.setColor(QPalette::ToolTipText, Qt::white);
	palette.setColor(QPalette::Text, Qt::white);
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::BrightText, Qt::red);
	palette.setColor(QPalette::Link, QColor(42, 130, 218));
	palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	palette.setColor(QPalette::HighlightedText, Qt::black);

    qapp.setStyle(QStyleFactory::create("fusion"));
	qapp.setPalette(palette);
}

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(resource);
	// Needed to ensure appropriate OpenGL context is created for VTK rendering.
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

	QApplication qapp(argc, argv);
	qapp.setApplicationName("dxfplotter");
	qapp.setApplicationDisplayName("dxfplotter");

	setDarkPalette(qapp);

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
