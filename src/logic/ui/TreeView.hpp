#pragma once

#include "ui_mainWindow.h"


class TreeItem
{
public:
	TreeItem(const QVector<QVariant> &data, TreeItem *parentItem = nullptr);
	virtual ~TreeItem();

	void appendChild(TreeItem *child);

	TreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	TreeItem *parentItem();

protected:
	QVector<TreeItem*>  m_childItems;
	QVector<QVariant>   m_itemData;
	TreeItem*           m_parentItem;
};



class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	TreeModel(QObject *parent = nullptr);
	virtual ~TreeModel() = 0;

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	virtual TreeItem* GetRootItem() const = 0;
};
