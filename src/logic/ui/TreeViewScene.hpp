#pragma once

#include <QMap>

#include "ui_mainWindow.h"
#include "logic/ui/TreeView.hpp"
#include "logic/Scene.hpp"


class TreeItemScene: public TreeItem
{
public:
	explicit TreeItemScene(const QVector<QVariant> &data, TreeItemScene *parentItem = nullptr);
	~TreeItemScene();

	void setPxPtr(void* ptr) { m_pxPtr = ptr; };
	inline void* getPxPtr() { return m_pxPtr; };
	void setPxTypeName(QString typeName) { m_pxTypeName = typeName; };
	inline QString getPxTypeName() const { return m_pxTypeName; };

private:
	void*     m_pxPtr;
	QString   m_pxTypeName;
};



class TreeModelScene : public TreeModel
{
	Q_OBJECT

public:
	explicit TreeModelScene(QObject *parent = nullptr);
	~TreeModelScene();

	void Setup(Scene* scene);
	virtual TreeItemScene* GetRootItem() const override;
	TreeItemScene* GetItem(const QModelIndex &index);
	QModelIndex FindShapeIndex(physx::PxShape* shape);

private:
	void setupModelData(Scene* scene);
	TreeItemScene* createChildNode(TreeItemScene *parent, QString& typeName, void* ptr, bool showNum=true);
	QModelIndex getModelIndex(TreeItemScene* item);

	TreeItemScene *m_RootItem;
	QMap<physx::PxShape*, QModelIndex>*  shape2ModelIndex;
};
