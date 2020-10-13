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

	inline QPushButton* GetPushButtonBitOrWord0()      const { return m_Ui->bitOrWord0PushButton; }
	inline QPushButton* GetPushButtonBitAndWord0()     const { return m_Ui->bitAndWord0PushButton; }
	inline QPushButton* GetPushButtonBitOrWord1()      const { return m_Ui->bitOrWord1PushButton; }
	inline QPushButton* GetPushButtonBitAndWord1()     const { return m_Ui->bitAndWord1PushButton; }
	inline QPushButton* GetPushButtonBitOrWord2()      const { return m_Ui->bitOrWord2PushButton; }
	inline QPushButton* GetPushButtonBitAndWord2()     const { return m_Ui->bitAndWord2PushButton; }
	inline QPushButton* GetPushButtonBitOrWord3()      const { return m_Ui->bitOrWord3PushButton; }
	inline QPushButton* GetPushButtonBitAndWord3()     const { return m_Ui->bitAndWord3PushButton; }

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

	void OnBitOrWord0PushButtonClick();
	void OnBitAndWord0PushButtonClick();
	void OnBitOrWord1PushButtonClick();
	void OnBitAndWord1PushButtonClick();
	void OnBitOrWord2PushButtonClick();
	void OnBitAndWord2PushButtonClick();
	void OnBitOrWord3PushButtonClick();
	void OnBitAndWord3PushButtonClick();

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
	void MainWindow::bitOpWords(QVector<QString>& words, BitOpType logicOpType);
};
