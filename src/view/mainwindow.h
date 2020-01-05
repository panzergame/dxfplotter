#pragma once

#include <core/path.h>

#include <ui_mainwindow.h>

#include <QMainWindow>

namespace View
{

class MainWindow : public QMainWindow, private Ui::MainWindow
{
private:
	Core::Paths m_paths;

	bool loadFile(const QString &fileName);
	void loadDxf(const QString &fileName);
	void loadPlot(const QString &fileName);

public:
	explicit MainWindow(const QString &fileName);

public Q_SLOTS:
	void openFile();
};

}
