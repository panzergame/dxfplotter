#pragma once

#include <ui_mainwindow.h>

#include <QMainWindow>

namespace View
{

class MainWindow : public QMainWindow, private Ui::MainWindow
{
private:
	void loadFile(const QString &fileName);
	void loadDxf(const QString &fileName);
	void loadPlot(const QString &fileName);

public:
	explicit MainWindow();

public Q_SLOTS:
	void openFile();
};

}
