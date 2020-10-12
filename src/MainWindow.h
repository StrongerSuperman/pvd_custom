#pragma once

#include <QMainWindow>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QStyle>
#include <QSize>
#include <QTimer>
#include <QLineEdit>
#include <QMessageBox>

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
	inline QComboBox*   GetComboBoxFilter()         const { return m_Ui->filterComboBox; }

	inline QLineEdit*   GetLineEditWord0()          const { return m_Ui->word0LineEdit; }
	inline QLineEdit*   GetLineEditWord1()          const { return m_Ui->word1LineEdit; }
	inline QLineEdit*   GetLineEditWord2()          const { return m_Ui->word2LineEdit; }
	inline QLineEdit*   GetLineEditWord3()          const { return m_Ui->word3LineEdit; }

	inline QPushButton* GetPushButtonLogicOrWord0()      const { return m_Ui->logicOrWord0PushButton; }
	inline QPushButton* GetPushButtonLogicAndWord0()     const { return m_Ui->logicAndWord0PushButton; }
	inline QPushButton* GetPushButtonLogicOrWord1()      const { return m_Ui->logicOrWord1PushButton; }
	inline QPushButton* GetPushButtonLogicAndWord1()     const { return m_Ui->logicAndWord1PushButton; }
	inline QPushButton* GetPushButtonLogicOrWord2()      const { return m_Ui->logicOrWord2PushButton; }
	inline QPushButton* GetPushButtonLogicAndWord2()     const { return m_Ui->logicAndWord2PushButton; }
	inline QPushButton* GetPushButtonLogicOrWord3()      const { return m_Ui->logicOrWord3PushButton; }
	inline QPushButton* GetPushButtonLogicAndWord3()     const { return m_Ui->logicAndWord3PushButton; }

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

	void OnShadeComboBoxIndexChanged(const QString &text);
	void OnFilterComboBoxIndexChanged(const QString &text);

	void OnLogicOrWord0PushButtonClick();
	void OnLogicAndWord0PushButtonClick();
	void OnLogicOrWord1PushButtonClick();
	void OnLogicAndWord1PushButtonClick();
	void OnLogicOrWord2PushButtonClick();
	void OnLogicAndWord2PushButtonClick();
	void OnLogicOrWord3PushButtonClick();
	void OnLogicAndWord3PushButtonClick();

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
	void MainWindow::logicOpWords(QVector<QString>& words, LogicOpType logicOpType);
};
