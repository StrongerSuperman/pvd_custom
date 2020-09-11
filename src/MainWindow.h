#pragma once

#include <QMainWindow>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QStyle>
#include <QSize>
#include <QTimer>

#include "ui_mainWindow.h"
#include "TreeViewScene.h"
#include "TreeViewAttr.h"
#include "Scene.h"


namespace Ui
{
	class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = NULL);
	~MainWindow();

	inline QAction*     GetActionOpen()          const { return m_Ui->actionopen; }
	inline QAction*     GetActionZoomToScene()   const { return m_Ui->actionZoom_to_scene; }
	inline GlWidget*    GetGlWidget()            const { return m_Ui->glWidget;      }
	inline QTreeView*   GetSceneTreeView()       const { return m_Ui->sceneTreeView; }
	inline QTreeView*   GetAttrTreeView()        const { return m_Ui->attrTreeView;  }

private slots:
	void OnInitialize();
	void OnOpenFileFolder();
	void OnZoomToScene();
	void OnShapePicked(physx::PxShape* shape);
	void OnSceneTreeViewClick(const QModelIndex& index);

private:
	Ui::MainWindow*  m_Ui;
	AttrTreeModel*   m_AttrTreeModel;
	SceneTreeModel*  m_SceneTreeModel;

	Scene*           m_Scene;

	void showItemAttr(void* ptr, const QString& typeName);
	void showSelectedShape(void* ptr, const QString& typeName);
	void deleteAttrTreeModel();
};
