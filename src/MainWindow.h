#pragma once

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QStyle>
#include <QSize>
#include <QTimer>

#include "ui_mainWindow.h"
#include "Scene.h"


namespace Ui
{
	class MainWindow;
}



class TreeItem
{
public:
	explicit TreeItem(const QVector<QVariant> &data, TreeItem *parentItem = nullptr, physx::PxBase* pxBasePtr = nullptr);
	~TreeItem();

	void SetPxBasePtr(physx::PxBase* ptr) { m_pxBasePtr = ptr; };
	inline physx::PxBase* GetPxBasePtr() { return m_pxBasePtr; };

	void appendChild(TreeItem *child);

	TreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	TreeItem *parentItem();

private:
	QVector<TreeItem*> m_childItems;
	QVector<QVariant>  m_itemData;
	TreeItem*          m_parentItem;
	physx::PxBase*     m_pxBasePtr;
};



class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit TreeModel(Scene* scene, QObject *parent = nullptr);
	~TreeModel();

	TreeItem* GetTreeItem(const QModelIndex &index);

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
	void setupModelData(Scene* scene, TreeItem *parent);

	TreeItem *m_RootItem;
};



class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = NULL);
	~MainWindow();

	inline GlWidget*    GetGlWidget()       const { return m_Ui->glWidget;      }
	inline QTreeView*   GetSceneTreeView()  const { return m_Ui->sceneTreeView; }
	inline QTreeView*   GetAttrTreeView()   const { return m_Ui->attrTreeView;  }

signals:
	void ShowItemAttr(physx::PxBase* m_pxBasePtr);

public slots:
	void OnSceneTreeViewClick(const QModelIndex& index);
	void OnShowItemAttr(physx::PxBase* m_pxBasePtr);

private:
	Ui::MainWindow* m_Ui;

private slots:
	void Initialize();
};
