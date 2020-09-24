#pragma once

#include <QMainWindow>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QStyle>
#include <QSize>
#include <QTimer>
#include <QLineEdit>

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

	inline QAction*     GetActionOpen()             const { return m_Ui->actionOpen; }
	inline QAction*     GetActionZoomToScene()      const { return m_Ui->actionZoomToScene; }
	inline QSlider*     GetSliderKeyMoveSpeed()     const { return m_Ui->sliderKeyMoveSpeed; }
	inline QSlider*     GetSliderMouseLeftSpeed()   const { return m_Ui->sliderMouseLeftSpeed; }
	inline QSlider*     GetSliderMouseRightSpeed()  const { return m_Ui->sliderMouseRightSpeed; }
	inline QSlider*     GetSliderMouseScrollSpeed() const { return m_Ui->sliderMouseScrollSpeed; }
	inline QComboBox*   GetComboBoxShade()          const { return m_Ui->shadeComboBox; }
	inline GlWidget*    GetGlWidget()               const { return m_Ui->glWidget;      }
	inline QTreeView*   GetSceneTreeView()          const { return m_Ui->sceneTreeView; }
	inline QTreeView*   GetAttrTreeView()           const { return m_Ui->attrTreeView;  }

private slots:
	void OnInitialize();
	void OnOpenFileFolder();
	void OnZoomToScene();
	void OnShapePicked(physx::PxShape* shape);
	void OnSceneTreeViewClick(const QModelIndex& index);
	void OnSliderKeyMoveSlide(int value);
	void OnSliderMouseLeftSlide(int value);
	void OnSliderMouseRightSlide(int value);
	void OnSliderMouseScrollSlide(int value);
	void OnComboBoxIndexChanged(const QString &text);

private:
	Ui::MainWindow*  m_Ui;
	Scene*           m_Scene;
	AttrTreeModel*   m_AttrTreeModel;
	SceneTreeModel*  m_SceneTreeModel;

	void initialize();
	void connectObject();
	void showItemAttr(void* ptr, const QString& typeName);
	void showSelectedShape(void* ptr, const QString& typeName);
	void deleteAttrTreeModel();
	void deleteSceneTreeModel();
};
