#include "TreeViewAttr.h"


AttrTreeItem::AttrTreeItem(const QVector<QVariant> &data, AttrTreeItem *parent)
	: m_itemData(data), m_parentItem(parent)
{
}

AttrTreeItem::~AttrTreeItem()
{
	qDeleteAll(m_childItems);
}

void AttrTreeItem::appendChild(AttrTreeItem *item)
{
	m_childItems.append(item);
}

AttrTreeItem *AttrTreeItem::child(int row)
{
	if (row < 0 || row >= m_childItems.size())
		return nullptr;
	return m_childItems.at(row);
}

int AttrTreeItem::childCount() const
{
	return m_childItems.count();
}

int AttrTreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<AttrTreeItem*>(this));

	return 0;
}

int AttrTreeItem::columnCount() const
{
	return m_itemData.count();
}

QVariant AttrTreeItem::data(int column) const
{
	if (column < 0 || column >= m_itemData.size())
		return QVariant();
	return m_itemData.at(column);
}

AttrTreeItem *AttrTreeItem::parentItem()
{
	return m_parentItem;
}



AttrTreeModel::AttrTreeModel(void* dataPtr, QString dataType, QObject *parent)
	: QAbstractItemModel(parent)
{
	m_RootItem = new AttrTreeItem({ tr("Name"), tr("Value") }, nullptr);
	setupModelData(dataPtr, dataType, m_RootItem);
}

AttrTreeModel::~AttrTreeModel()
{
	delete m_RootItem;
}

QModelIndex AttrTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	AttrTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = m_RootItem;
	else
		parentItem = static_cast<AttrTreeItem*>(parent.internalPointer());

	AttrTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}

QModelIndex AttrTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	AttrTreeItem *childItem = static_cast<AttrTreeItem*>(index.internalPointer());
	AttrTreeItem *parentItem = childItem->parentItem();

	if (parentItem == m_RootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int AttrTreeModel::rowCount(const QModelIndex &parent) const
{
	AttrTreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = m_RootItem;
	else
		parentItem = static_cast<AttrTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int AttrTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<AttrTreeItem*>(parent.internalPointer())->columnCount();
	return m_RootItem->columnCount();
}

QVariant AttrTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	AttrTreeItem *item = static_cast<AttrTreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags AttrTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index);
}

QVariant AttrTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_RootItem->data(section);

	return QVariant();
}


void AttrTreeModel::setupModelData(void* dataPtr, QString dataType, AttrTreeItem *parent)
{
	if (dataType == QString("PxScene"))
	{
		auto scene = reinterpret_cast<physx::PxScene*>(dataPtr);
		(void)(scene);
		return;
	}
	else if (dataType == QString("PxRigidStatics"))
	{
		return;
	}
	else if (dataType == QString("PxRigidDynamics"))
	{
		return;
	}
	else if (dataType == QString("PxRigidStatic"))
	{
		auto actor = reinterpret_cast<physx::PxRigidStatic*>(dataPtr);

		// GlobalPose
		{
			auto name0 = QString("GlobalPose");
			QString value0;
			auto globalPoseNode = createChildNode(parent, name0, value0);
			{
				auto name1 = QString("Pose");
				QString value1;
				auto pose = actor->getGlobalPose().p;
				value1.sprintf("[%.3f, %.3f, %.3f]", pose.x, pose.y, pose.z);
				createChildNode(globalPoseNode, name1, value1);
			}
			{
				auto name2 = QString("Rotate");
				QString value2;
				auto rotate = actor->getGlobalPose().q;
				value2.sprintf("[%.3f, %.3f, %.3f, %.3f]", rotate.x, rotate.y, rotate.z, rotate.w);
				createChildNode(globalPoseNode, name2, value2);
			}
		}
	}
	else if (dataType == QString("PxRigidDynamic"))
	{
		auto actor = reinterpret_cast<physx::PxRigidDynamic*>(dataPtr);

		// GlobalPose
		{
			auto name0 = QString("GlobalPose");
			QString value0;
			auto globalPoseNode = createChildNode(parent, name0, value0);
			{
				auto name1 = QString("Pose");
				QString value1;
				auto pose = actor->getGlobalPose().p;
				value1.sprintf("[%.3f, %.3f, %.3f]", pose.x, pose.y, pose.z);
				createChildNode(globalPoseNode, name1, value1);
			}
			{
				auto name2 = QString("Rotate");
				QString value2;
				auto rotate = actor->getGlobalPose().q;
				value2.sprintf("[%.3f, %.3f, %.3f, %.3f]", rotate.x, rotate.y, rotate.z, rotate.w);
				createChildNode(globalPoseNode, name2, value2);
			}
		}
	}
	else if (dataType == QString("PxShape"))
	{
		auto shape = reinterpret_cast<physx::PxShape*>(dataPtr);

		// SimulationFilterData
		{
			auto name0 = QString("SimulationFilterData");
			QString value0;
			auto data = shape->getSimulationFilterData();
			value0.sprintf("[0x%08X, 0x%08X, 0x%08X, 0x%08X]", data.word0, data.word1, data.word2, data.word3);
			createChildNode(parent, name0, value0);
		}

		// QueryFilterData
		{
			auto name1 = QString("QueryFilterData");
			QString value1;
			auto data = shape->getQueryFilterData();
			value1.sprintf("[0x%08X, 0x%08X, 0x%08X, 0x%08X]", data.word0, data.word1, data.word2, data.word3);
			createChildNode(parent, name1, value1);
		}
	}
}

AttrTreeItem* AttrTreeModel::createChildNode(AttrTreeItem *parent, QString& name, QString& value)
{
	QString nameTitle(name);
	auto nameBa = nameTitle.toLatin1();
	auto nameChar = nameBa.data();

	QString valueTitle(value);
	auto valueBa = valueTitle.toLatin1();
	auto valueChar = valueBa.data();

	auto item = new AttrTreeItem({ tr(nameChar), tr(valueChar) }, parent);
	parent->appendChild(item);
	return item;
}

AttrTreeItem* AttrTreeModel::getAttrTreeItem(const QModelIndex &index)
{
	return static_cast<AttrTreeItem*>(index.internalPointer());
}
