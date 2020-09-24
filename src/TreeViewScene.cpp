#include "TreeViewScene.h"


SceneTreeItem::SceneTreeItem(const QVector<QVariant> &data, SceneTreeItem *parent)
	: m_itemData(data), m_parentItem(parent), m_pxPtr(nullptr)
{
}

SceneTreeItem::~SceneTreeItem()
{
	qDeleteAll(m_childItems);
}

void SceneTreeItem::appendChild(SceneTreeItem *item)
{
	m_childItems.append(item);
}

SceneTreeItem *SceneTreeItem::child(int row)
{
	if (row < 0 || row >= m_childItems.size())
		return nullptr;
	return m_childItems.at(row);
}

int SceneTreeItem::childCount() const
{
	return m_childItems.count();
}

int SceneTreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<SceneTreeItem*>(this));

	return 0;
}

int SceneTreeItem::columnCount() const
{
	return m_itemData.count();
}

QVariant SceneTreeItem::data(int column) const
{
	if (column < 0 || column >= m_itemData.size())
		return QVariant();
	return m_itemData.at(column);
}

SceneTreeItem *SceneTreeItem::parentItem()
{
	return m_parentItem;
}



SceneTreeModel::SceneTreeModel(Scene* scene, QObject *parent)
	: QAbstractItemModel(parent),
	shape2ModelIndex(new QMap<physx::PxShape*, QModelIndex>)
{
	m_RootItem = new SceneTreeItem({ tr("Scene") }, nullptr);
	setupModelData(scene, m_RootItem);
}

SceneTreeModel::~SceneTreeModel()
{
	delete m_RootItem;
}

QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	SceneTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = m_RootItem;
	else
		parentItem = static_cast<SceneTreeItem*>(parent.internalPointer());

	SceneTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}

QModelIndex SceneTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	SceneTreeItem *childItem = static_cast<SceneTreeItem*>(index.internalPointer());
	SceneTreeItem *parentItem = childItem->parentItem();

	if (parentItem == m_RootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int SceneTreeModel::rowCount(const QModelIndex &parent) const
{
	SceneTreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = m_RootItem;
	else
		parentItem = static_cast<SceneTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int SceneTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<SceneTreeItem*>(parent.internalPointer())->columnCount();
	return m_RootItem->columnCount();
}

QVariant SceneTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	SceneTreeItem *item = static_cast<SceneTreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags SceneTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index);
}

QVariant SceneTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_RootItem->data(section);

	return QVariant();
}


void SceneTreeModel::setupModelData(Scene* scene, SceneTreeItem *parent)
{
	// scene node
	auto sceneTypeName = QString("PxScene");
	auto sceneItem = createChildNode(parent, sceneTypeName,
		static_cast<void*>(scene->GetPhysics()->GetPxScene()));

	// statics node
	auto staticsTypeName = QString("PxRigidStatics");
	auto staticsItem = createChildNode(sceneItem, staticsTypeName, nullptr, false);

	// dynamics node
	auto dynamicsTypeName = QString("PxRigidDynamics");
	auto dynamicsItem = createChildNode(sceneItem, dynamicsTypeName, nullptr, false);

	for each(auto &actor in scene->GetActors())
	{
		// actor node
		SceneTreeItem *actorItem;
		if (actor->is<physx::PxRigidStatic>())
		{
			auto actorTypeName = QString("PxRigidStatic");
			actorItem = createChildNode(staticsItem, actorTypeName, static_cast<void*>(actor));
		}
		else if (actor->is<physx::PxRigidDynamic>())
		{
			auto actorTypeName = QString("PxRigidDynamic");
			actorItem = createChildNode(dynamicsItem, actorTypeName, static_cast<void*>(actor));
		}

		auto nbShapes = actor->getNbShapes();
		std::vector<physx::PxShape*> shapes(nbShapes);
		actor->getShapes(&shapes[0], nbShapes);
		for each(auto &shape in shapes)
		{
			// shape node
			auto shapeTypeName = QString("PxShape");
			auto shape_item = createChildNode(actorItem, shapeTypeName, static_cast<void*>(shape));
			shape2ModelIndex->insert(shape, getModelIndex(shape_item));
		}
	}
}

SceneTreeItem* SceneTreeModel::createChildNode(SceneTreeItem *parent, QString& typeName, void* ptr, bool showNum)
{
	QString title(typeName);
	if (showNum)
	{
		title.append(" [%d]");
		title.sprintf(title.toStdString().c_str(), parent->childCount());
	}
	auto ba = title.toLatin1();
	auto titleChar = ba.data();
	auto item = new SceneTreeItem({ tr(titleChar) }, parent);
	item->setPxTypeName(typeName);
	item->setPxPtr(ptr);
	parent->appendChild(item);
	return item;
}

SceneTreeItem* SceneTreeModel::getItem(const QModelIndex &index)
{
	return static_cast<SceneTreeItem*>(index.internalPointer());
}

QModelIndex SceneTreeModel::getModelIndex(SceneTreeItem* item)
{
	return createIndex(item->row(), 0, item);
}

QModelIndex SceneTreeModel::findShapeIndex(physx::PxShape* shape)
{
	return shape2ModelIndex->value(shape, QModelIndex());
}
