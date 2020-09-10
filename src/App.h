#include <QApplication>

#include "MainWindow.h"


class App: public QApplication
{
	Q_OBJECT

public:
	App(int argc, char *argv[]);
	~App();

	MainWindow*   m_MainWindow;
};
