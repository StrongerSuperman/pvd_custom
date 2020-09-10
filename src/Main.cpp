#include <QApplication>
#include <QCoreApplication>

#include <QVector>

#include "MainWindow.h"
#include "GlWidget.h"
#include "Scene.h"


int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	application.processEvents();

	Scene scene;
	scene.Load({ "./res/test2.col_xml" });

	MainWindow mainWindow;
	GlWidget *glWidget = mainWindow.GetGlWidget();
	glWidget->AddScene(&scene);
	glWidget->ActiveScene(0);

	QTreeView* sceneTreeView = mainWindow.GetSceneTreeView();
	SceneTreeModel  sceneTreeModel(&scene);
	sceneTreeView->setModel(&sceneTreeModel);

	application.exec();
}
