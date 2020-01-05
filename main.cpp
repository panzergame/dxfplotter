#include <view/mainwindow.h>

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	View::MainWindow window;

	app.exec();
}
