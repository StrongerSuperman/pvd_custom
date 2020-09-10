#include "MainWindow.h"


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
	deleteAttrTreeModel();
}


void MainWindow::Initialize()
{
	QObject::connect(
		GetSceneTreeView(), SIGNAL(clicked(const QModelIndex)),
		this, SLOT(OnSceneTreeViewClick(const QModelIndex))
	);

	QObject::connect(
		GetSceneTreeView(), SIGNAL(doubleClicked(const QModelIndex)),
		this, SLOT(OnSceneTreeViewClick(const QModelIndex))
	);

	QObject::connect(
		GetGlWidget(), SIGNAL(ShapePicked(physx::PxShape*)),
		this, SLOT(OnShapePicked(physx::PxShape*))
	);

	GetSceneTreeView()->setExpandsOnDoubleClick(true);
	GetAttrTreeView()->setExpandsOnDoubleClick(true);

	show();
}

void MainWindow::OnShapePicked(physx::PxShape* shape)
{
	showItemAttr(static_cast<void*>(shape), "PxShape");
}

void MainWindow::OnSceneTreeViewClick(const QModelIndex& index)
{
	auto sceneTreeModel = reinterpret_cast<SceneTreeModel*>(GetSceneTreeView()->model());
	auto treeItem = sceneTreeModel->getSceneTreeItem(index);
	auto ptr = treeItem->getPxPtr();
	auto typeName = treeItem->getPxTypeName();
	showItemAttr(ptr, typeName);
	showSelectedShape(ptr, typeName);
}

void MainWindow::showItemAttr(void* ptr, const QString& typeName)
{
	if (!ptr)
	{
		deleteAttrTreeModel();
		return;
	}

	deleteAttrTreeModel();
	m_AttrTreeModel = new AttrTreeModel(ptr, typeName);
	GetAttrTreeView()->setModel(m_AttrTreeModel);
	GetAttrTreeView()->expandAll();
}

void MainWindow::showSelectedShape(void* ptr, const QString& typeName)
{
	std::vector<physx::PxShape*> shapes;

	if (typeName == QString("PxRigidStatic"))
	{
		auto actor = reinterpret_cast<physx::PxRigidStatic*>(ptr);

		auto nbShapes = actor->getNbShapes();
		shapes.resize(nbShapes);
		actor->getShapes(&shapes[0], nbShapes);
	}
	else if (typeName == QString("PxRigidDynamic"))
	{
		auto actor = reinterpret_cast<physx::PxRigidDynamic*>(ptr);

		auto nbShapes = actor->getNbShapes();
		shapes.resize(nbShapes);
		actor->getShapes(&shapes[0], nbShapes);
	}
	else if (typeName == QString("PxShape"))
	{
		auto shape = reinterpret_cast<physx::PxShape*>(ptr);
		shapes.push_back(shape);
	}

	GetGlWidget()->SetPickedShapes(shapes);
}

void MainWindow::deleteAttrTreeModel()
{
	if (m_AttrTreeModel)
	{
		delete m_AttrTreeModel;
		m_AttrTreeModel = nullptr;
	}
}
