#pragma once

#include "ui_mainWindow.h"
#include "logic/ui/TreeView.hpp"


class TreeItemAttr: public TreeItem
{
public:
	explicit TreeItemAttr(const QVector<QVariant> &data, TreeItemAttr *parentItem = nullptr);
	~TreeItemAttr();
};



class TreeModelAttr : public TreeModel
{
	Q_OBJECT

public:
	explicit TreeModelAttr(QObject *parent = nullptr);
	~TreeModelAttr();

	void Setup(void* dataPtr, QString dataType);
	virtual TreeItemAttr* GetRootItem() const override;
	TreeItemAttr* GetItem(const QModelIndex &index);
	
private:
	void setupModelData(void* dataPtr, QString dataType);
	TreeItemAttr* createChildNode(TreeItemAttr *parent, QString& name, QString& value);

	TreeItemAttr *m_RootItem;
};
