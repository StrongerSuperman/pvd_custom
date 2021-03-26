#include <QApplication>

#include "logic/ui/MainWindow.hpp"


class App: public QApplication
{
	Q_OBJECT

public:
	App(int argc, char *argv[]);
	~App();

	MainWindow*   m_MainWindow;
};
