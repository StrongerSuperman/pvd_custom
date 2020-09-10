#pragma once

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
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

	inline GlWidget*    GetGlWidget()       const { return m_Ui->glWidget;      }
	inline QTreeView*   GetSceneTreeView()  const { return m_Ui->sceneTreeView; }
	inline QTreeView*   GetAttrTreeView()   const { return m_Ui->attrTreeView;  }

public slots:
	void OnShapePicked(physx::PxShape* shape);

private slots:
	void Initialize();
	void OnSceneTreeViewClick(const QModelIndex& index);

private:
	Ui::MainWindow* m_Ui;
	AttrTreeModel*  m_AttrTreeModel;

	void showItemAttr(void* ptr, const QString& typeName);
	void showSelectedShape(void* ptr, const QString& typeName);
	void deleteAttrTreeModel();
};
