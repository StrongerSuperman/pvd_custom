#pragma once

#include "ui_mainWindow.h"


class AttrTreeItem
{
public:
	explicit AttrTreeItem(const QVector<QVariant> &data, AttrTreeItem *parentItem = nullptr);
	~AttrTreeItem();

	void appendChild(AttrTreeItem *child);

	AttrTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	AttrTreeItem *parentItem();

private:
	QVector<AttrTreeItem*> m_childItems;
	QVector<QVariant>       m_itemData;
	AttrTreeItem*          m_parentItem;
};



class AttrTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit AttrTreeModel(void* dataPtr, QString dataType, QObject *parent = nullptr);
	~AttrTreeModel();

	AttrTreeItem* getAttrTreeItem(const QModelIndex &index);

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
	void setupModelData(void* dataPtr, QString dataType, AttrTreeItem *parent);
	AttrTreeItem* createChildNode(AttrTreeItem *parent, QString& name, QString& value);

	AttrTreeItem *m_RootItem;
};
