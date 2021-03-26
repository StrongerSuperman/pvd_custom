#include "logic/ui/TreeViewScene.hpp"


TreeItemScene::TreeItemScene(const QVector<QVariant> &data, TreeItemScene *parent)
	: TreeItem(data, parent), m_pxPtr(nullptr)
{
}

TreeItemScene::~TreeItemScene()
{
}



TreeModelScene::TreeModelScene(QObject *parent)
	: TreeModel(parent)
{
	shape2ModelIndex = new QMap<physx::PxShape*, QModelIndex>;
}

TreeModelScene::~TreeModelScene()
{
	delete m_RootItem;
	delete shape2ModelIndex;
}

void TreeModelScene::Setup(Scene* scene)
{
	m_RootItem = new TreeItemScene({ tr("Scene") }, nullptr);
	setupModelData(scene);
}

TreeItemScene* TreeModelScene::GetRootItem() const
{
	return m_RootItem;
}

TreeItemScene* TreeModelScene::GetItem(const QModelIndex &index)
{
	return static_cast<TreeItemScene*>(index.internalPointer());
}

QModelIndex TreeModelScene::FindShapeIndex(physx::PxShape* shape)
{
	return shape2ModelIndex->value(shape, QModelIndex());
}


void TreeModelScene::setupModelData(Scene* scene)
{
	auto parent = m_RootItem;

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
		TreeItemScene *actorItem;
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

TreeItemScene* TreeModelScene::createChildNode(TreeItemScene *parent, QString& typeName, void* ptr, bool showNum)
{
	QString title(typeName);
	if (showNum)
	{
		title.append(" [%d]");
		title.sprintf(title.toStdString().c_str(), parent->childCount());
	}
	auto ba = title.toLatin1();
	auto titleChar = ba.data();
	auto item = new TreeItemScene({ tr(titleChar) }, parent);
	item->setPxTypeName(typeName);
	item->setPxPtr(ptr);
	parent->appendChild(item);
	return item;
}

QModelIndex TreeModelScene::getModelIndex(TreeItemScene* item)
{
	return createIndex(item->row(), 0, item);
}
