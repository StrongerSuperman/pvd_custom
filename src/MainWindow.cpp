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
	delete m_AttrTreeModel;
}


void MainWindow::Initialize()
{
	show();

	QObject::connect(
		GetSceneTreeView(), SIGNAL(clicked(const QModelIndex)),
		this, SLOT(OnSceneTreeViewClick(const QModelIndex))
	);

	QObject::connect(
		GetSceneTreeView(), SIGNAL(doubleClicked(const QModelIndex)),
		this, SLOT(OnSceneTreeViewClick(const QModelIndex))
	);

	GetSceneTreeView()->setExpandsOnDoubleClick(true);
	GetAttrTreeView()->setExpandsOnDoubleClick(true);
}

void MainWindow::OnSceneTreeViewClick(const QModelIndex& index)
{
	auto sceneTreeModel = reinterpret_cast<SceneTreeModel*>(GetSceneTreeView()->model());
	auto treeItem = sceneTreeModel->getSceneTreeItem(index);
	showItemAttr(treeItem->getPxPtr(), treeItem->getPxTypeName());
}

void MainWindow::showItemAttr(void* m_pxPtr, const QString& typeName)
{
	if (!m_pxPtr)
	{
		return;
	}

	if (m_AttrTreeModel)
	{
		delete m_AttrTreeModel;
	}
	m_AttrTreeModel = new AttrTreeModel(m_pxPtr, typeName);
	GetAttrTreeView()->setModel(m_AttrTreeModel);
	GetAttrTreeView()->expandAll();
}
