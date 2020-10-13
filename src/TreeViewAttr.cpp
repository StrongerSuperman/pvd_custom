#include "TreeViewAttr.h"


TreeItemAttr::TreeItemAttr(const QVector<QVariant> &data, TreeItemAttr *parent)
	: TreeItem(data, parent)
{
}

TreeItemAttr::~TreeItemAttr()
{
}



TreeModelAttr::TreeModelAttr(QObject *parent)
	: TreeModel(parent)
{
}

TreeModelAttr::~TreeModelAttr()
{
	delete m_RootItem;
}

void TreeModelAttr::Setup(void* dataPtr, QString dataType)
{
	m_RootItem = new TreeItemAttr({ tr("Name"), tr("Value") }, nullptr);
	setupModelData(dataPtr, dataType);
}

TreeItemAttr* TreeModelAttr::GetRootItem() const
{
	return m_RootItem;
}

TreeItemAttr* TreeModelAttr::GetItem(const QModelIndex &index)
{
	return static_cast<TreeItemAttr*>(index.internalPointer());
}


void TreeModelAttr::setupModelData(void* dataPtr, QString dataType)
{
	auto parent = m_RootItem;

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

TreeItemAttr* TreeModelAttr::createChildNode(TreeItemAttr *parent, QString& name, QString& value)
{
	QString nameTitle(name);
	auto nameBa = nameTitle.toLatin1();
	auto nameChar = nameBa.data();

	QString valueTitle(value);
	auto valueBa = valueTitle.toLatin1();
	auto valueChar = valueBa.data();

	auto item = new TreeItemAttr({ tr(nameChar), tr(valueChar) }, parent);
	parent->appendChild(item);
	return item;
}
