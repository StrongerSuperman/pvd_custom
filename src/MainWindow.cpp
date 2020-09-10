#include "MainWindow.h"


TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent, physx::PxBase* pxBasePtr)
	: m_itemData(data), m_parentItem(parent), m_pxBasePtr(pxBasePtr)
{
}

TreeItem::~TreeItem()
{
	qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
	m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
	if (row < 0 || row >= m_childItems.size())
		return nullptr;
	return m_childItems.at(row);
}

int TreeItem::childCount() const
{
	return m_childItems.count();
}

int TreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

int TreeItem::columnCount() const
{
	return m_itemData.count();
}

QVariant TreeItem::data(int column) const
{
	if (column < 0 || column >= m_itemData.size())
		return QVariant();
	return m_itemData.at(column);
}

TreeItem *TreeItem::parentItem()
{
	return m_parentItem;
}



TreeModel::TreeModel(Scene* scene, QObject *parent)
	: QAbstractItemModel(parent)
{
	m_RootItem = new TreeItem({ tr("Scene") }, nullptr);
	setupModelData(scene, m_RootItem);
}

TreeModel::~TreeModel()
{
	delete m_RootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = m_RootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parentItem();

	if (parentItem == m_RootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = m_RootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	return m_RootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_RootItem->data(section);

	return QVariant();
}

void TreeModel::setupModelData(Scene* scene, TreeItem *parent)
{
	TreeItem* staticsItem = new TreeItem({ tr("RigidStatics") }, parent);
	parent->appendChild(staticsItem);

	TreeItem* dynamicsItem = new TreeItem({ tr("RigidDynamics") }, parent);
	parent->appendChild(dynamicsItem);

	for each(auto &actor in scene->GetActors())
	{
		QString title;
		TreeItem* actorParentItem;
		if (actor->is<physx::PxRigidStatic>())
		{
			title.sprintf("RigidStatic [%d]", staticsItem->childCount());
			actorParentItem = staticsItem;
		}
		else if (actor->is<physx::PxRigidDynamic>())
		{
			title.sprintf("RigidDynamic [%d]", dynamicsItem->childCount());
			actorParentItem = dynamicsItem;
		}

		QByteArray ba = title.toLatin1();
		char* titleChar = ba.data();
		auto actorPtr = reinterpret_cast<physx::PxBase*>(actor);
		TreeItem* actorItem = new TreeItem({ tr(titleChar) }, actorParentItem, actorPtr);
		actorParentItem->appendChild(actorItem);

		auto nbShapes = actor->getNbShapes();
		std::vector<physx::PxShape*> shapes(nbShapes);
		actor->getShapes(&shapes[0], nbShapes);
		for each(auto &shape in shapes)
		{
			QString title;
			title.sprintf("Shape [%d]", actorItem->childCount());
			QByteArray ba = title.toLatin1();
			char* titleChar = ba.data();
			auto shapePtr = reinterpret_cast<physx::PxBase*>(shape);
			TreeItem* shapeItem = new TreeItem({ tr(titleChar) }, actorItem, shapePtr);
			actorItem->appendChild(shapeItem);
		}
	}
}

TreeItem* TreeModel::GetTreeItem(const QModelIndex &index)
{
	return static_cast<TreeItem*>(index.internalPointer());
}


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_Ui(new Ui::MainWindow)
{
	m_Ui->setupUi(this);

	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(1000, 610), qApp->desktop()->availableGeometry()));

	QTimer::singleShot(2000, this, SLOT(Initialize()));
}

MainWindow::~MainWindow()
{
	delete m_Ui;
}

void MainWindow::OnSceneTreeViewClick(const QModelIndex& index)
{
	auto scenetreeView = GetSceneTreeView();
	auto sceneTreeModel = reinterpret_cast<TreeModel*>(scenetreeView->model());
	emit ShowItemAttr(sceneTreeModel->GetTreeItem(index)->GetPxBasePtr());
}

void MainWindow::OnShowItemAttr(physx::PxBase* m_pxBasePtr)
{
	auto a = 1;
	/*if (m_pxBasePtr->is<physx::PxScene>())
	{
		auto scene = m_pxBasePtr->is<physx::PxScene>();
	}
	else if (m_pxBasePtr->is<physx::PxRigidStatic>())
	{
		auto rigidStatic = m_pxBasePtr->is<physx::PxRigidStatic>();
	}
	else if (m_pxBasePtr->is<physx::PxRigidDynamic>())
	{
		auto rigidDynamic = m_pxBasePtr->is<physx::PxRigidDynamic>();
	}
	else if (m_pxBasePtr->is<physx::PxShape>())
	{
		auto shape = m_pxBasePtr->is<physx::PxShape>();
	}*/
}

void MainWindow::Initialize()
{
	show();

	QObject::connect(
		GetSceneTreeView(), SIGNAL(clicked(const QModelIndex)),
		this, SLOT(OnSceneTreeViewClick(const QModelIndex))
	);

	QObject::connect(
		this, SIGNAL(ShowItemAttr(physx::PxBase* m_pxBasePtr)),
		this, SLOT(OnShowItemAttr(physx::PxBase* m_pxBasePtr))
	);
}
