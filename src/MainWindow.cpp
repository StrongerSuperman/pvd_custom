#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_Ui(new Ui::MainWindow),
	m_AttrTreeModel(nullptr),
	m_SceneTreeModel(nullptr),
	m_Scene(nullptr)
{
	m_Ui->setupUi(this);

	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(1000, 610), qApp->desktop()->availableGeometry()));

	QTimer::singleShot(2000, this, SLOT(OnInitialize()));
}

MainWindow::~MainWindow()
{
	delete m_Ui;
	deleteAttrTreeModel();
}


void MainWindow::OnInitialize()
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

	QObject::connect(
		GetActionOpen(), SIGNAL(triggered()),
		this, SLOT(OnOpenFileFolder())
	);

	QObject::connect(
		GetActionZoomToScene(), SIGNAL(triggered()),
		this, SLOT(OnZoomToScene())
	);

	GetSceneTreeView()->setExpandsOnDoubleClick(true);
	GetAttrTreeView()->setExpandsOnDoubleClick(true);

	show();
}

void MainWindow::OnOpenFileFolder()
{
	auto fileName = QFileDialog::getOpenFileName(
		this,
		tr("open a collection file."),
		"D:/",
		tr("Collection Files(*.col_xml)")
	);
	if (fileName.isEmpty())
	{
		return;
	}

	if (m_Scene)
	{
		delete m_Scene;
	}
	m_Scene = new Scene();
	const auto fileList = { fileName.toStdString() };
	m_Scene->Load(fileList);

	GetGlWidget()->SetScene(m_Scene);
	m_SceneTreeModel = new SceneTreeModel(m_Scene);
	GetSceneTreeView()->setModel(m_SceneTreeModel);
}

void MainWindow::OnZoomToScene()
{
	GetGlWidget()->ResetCamera();
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
