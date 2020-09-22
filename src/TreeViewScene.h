#pragma once

#include <QMap>

#include "ui_mainWindow.h"
#include "Scene.h"


class SceneTreeItem
{
public:
	explicit SceneTreeItem(const QVector<QVariant> &data, SceneTreeItem *parentItem = nullptr);
	~SceneTreeItem();

	void appendChild(SceneTreeItem *child);

	SceneTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	SceneTreeItem *parentItem();

	void setPxPtr(void* ptr) { m_pxPtr = ptr; };
	inline void* getPxPtr() { return m_pxPtr; };
	void setPxTypeName(QString typeName) { m_pxTypeName = typeName; };
	inline QString getPxTypeName() const { return m_pxTypeName; };

private:
	QVector<SceneTreeItem*> m_childItems;
	QVector<QVariant>       m_itemData;
	SceneTreeItem*          m_parentItem;
	void*                   m_pxPtr;
	QString                 m_pxTypeName;
};



class SceneTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit SceneTreeModel(Scene* scene, QObject *parent = nullptr);
	~SceneTreeModel();

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	SceneTreeItem* getItem(const QModelIndex &index);
	QModelIndex getModelIndex(SceneTreeItem* item);
	QModelIndex findShapeIndex(physx::PxShape* shape);

private:
	void setupModelData(Scene* scene, SceneTreeItem *parent);
	SceneTreeItem* createChildNode(SceneTreeItem *parent, QString& typeName, void* ptr, bool showNum=true);

	SceneTreeItem*                           m_RootItem;
	QMap<physx::PxShape*, QModelIndex>*      shape2ModelIndex;
};
