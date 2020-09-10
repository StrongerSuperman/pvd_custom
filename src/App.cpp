#include "App.h"

App::App(int argc, char *argv[]):
	QApplication(argc, argv)
{
	m_MainWindow = new MainWindow();
}

App::~App()
{
	delete m_MainWindow;
}
