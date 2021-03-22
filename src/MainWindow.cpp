#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_Ui(new Ui::MainWindow),
	m_AttrTreeModel(nullptr),
	m_SceneTreeModel(nullptr),
	m_Scene(nullptr)
{
	initialize();
}

MainWindow::~MainWindow()
{
	delete m_Ui;
	deleteAttrTreeModel();
	deleteSceneTreeModel();
	delete m_Scene;
}



void MainWindow::OnInitialize()
{
	connectObject();

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
	m_Scene = new Scene;
	const auto fileList = { fileName.toStdString() };
	m_Scene->Load(fileList);
	GetGlWidget()->SetScene(m_Scene);

	GetComboBoxShade()->setCurrentIndex(1);

	// init scene shade config
	OnShadeComboBoxIndexChanged(GetComboBoxShade()->currentText());
	OnFilterComboBoxIndexChanged(GetComboBoxFilter()->currentText());

	m_SceneTreeModel = new TreeModelScene();
	m_SceneTreeModel->Setup(GetGlWidget()->GetActiveScene());
	GetSceneTreeView()->setModel(m_SceneTreeModel);
	GetSceneTreeView()->setExpandsOnDoubleClick(true);
	GetAttrTreeView()->setExpandsOnDoubleClick(true);
}

void MainWindow::OnZoomToScene()
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}
	auto sceneBox = scene->GetAABB();
	scene->GetCamera()->ZoomToBox(sceneBox);
}


void MainWindow::OnShapePicked(physx::PxShape* shape)
{
	if (!shape)
	{
		return;
	}

	auto modelIndex = m_SceneTreeModel->FindShapeIndex(shape);
	GetSceneTreeView()->setCurrentIndex(modelIndex);
	OnSceneTreeViewClick(modelIndex);
}

void MainWindow::OnSceneTreeViewClick(const QModelIndex& index)
{
	auto model = dynamic_cast<TreeModelScene*>(GetSceneTreeView()->model());
	auto item = model->GetItem(index);
	auto ptr = item->getPxPtr();
	auto typeName = item->getPxTypeName();
	showItemAttr(ptr, typeName);
	showSelectedShape(ptr, typeName);
}


void MainWindow::OnSliderKeyMoveSlide(int value)
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}
	scene->GetCamera()->SetKeyMoveSpeed(static_cast<float>(value));
}

void MainWindow::OnSliderMouseLeftSlide(int value)
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}
	scene->GetCamera()->SetMouseLeftSpeed(static_cast<float>(value));
}

void MainWindow::OnSliderMouseRightSlide(int value)
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}
	scene->GetCamera()->SetMouseRightSpeed(static_cast<float>(value));
}

void MainWindow::OnSliderMouseScrollSlide(int value)
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}
	scene->GetCamera()->SetMouseScrollSpeed(static_cast<float>(value));
}


void MainWindow::OnShadeComboBoxIndexChanged(const QString &text)
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}
	if (text == "simulation_group")
	{
		scene->SetShadeType(SimulationGroup);
	}
	else if(text == "simulation_color")
	{
		scene->SetShadeType(SimulationColor);
	}
	else if (text == "query_group")
	{
		scene->SetShadeType(QueryGroup);
	}
	else if (text == "query_color")
	{
		scene->SetShadeType(QueryColor);
	}
	GetGlWidget()->update();
}

void MainWindow::OnFilterComboBoxIndexChanged(const QString &text)
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}
	if (text == "simulation_filter")
	{
		scene->SetFilterType(Simulation);
	}
	else if (text == "query_filter")
	{
		scene->SetFilterType(Query);
	}
	GetGlWidget()->update();
}



void MainWindow::OnBitOrWord0PushButtonClick()
{
	QVector<QString> words = { GetLineEditWord0()->text(), "", "",  "" };
	bitOpWords(words, Or);
}

void MainWindow::OnBitAndWord0PushButtonClick()
{
	QVector<QString> words = { GetLineEditWord0()->text(), "", "", "" };
	bitOpWords(words, And);
}

void MainWindow::OnBitOrWord1PushButtonClick()
{
	QVector<QString> words = { "", GetLineEditWord1()->text(), "", "" };
	bitOpWords(words, Or);
}

void MainWindow::OnBitAndWord1PushButtonClick()
{
	QVector<QString> words = { "", GetLineEditWord1()->text(), "", "" };
	bitOpWords(words, And);
}

void MainWindow::OnBitOrWord2PushButtonClick()
{
	QVector<QString> words = { "", "", GetLineEditWord2()->text(), "" };
	bitOpWords(words, Or);
}

void MainWindow::OnBitAndWord2PushButtonClick()
{
	QVector<QString> words = { "", "", GetLineEditWord2()->text(), "" };
	bitOpWords(words, And);
}

void MainWindow::OnBitOrWord3PushButtonClick()
{
	QVector<QString> words = { "", "", "", GetLineEditWord3()->text() };
	bitOpWords(words, Or);
}

void MainWindow::OnBitAndWord3PushButtonClick()
{
	QVector<QString> words = { "", "", "", GetLineEditWord3()->text() };
	bitOpWords(words, And);
}



void MainWindow::initialize()
{
	m_Ui->setupUi(this);
	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(1000, 610), qApp->desktop()->availableGeometry()));

	// set theme
	QString theme_filename = QCoreApplication::applicationDirPath() + "/../../res/theme/dark.qss";
	QFile file(theme_filename);
	if (file.open(QFile::ReadOnly))
	{
		QString styleSheet = QLatin1String(file.readAll());
		qApp->setStyleSheet(styleSheet);
		file.close();
	}

	// shade comboBox
	GetComboBoxShade()->addItem(QWidget::tr("simulation_group"));
	GetComboBoxShade()->addItem(QWidget::tr("simulation_color"));
	GetComboBoxShade()->addItem(QWidget::tr("query_group"));
	GetComboBoxShade()->addItem(QWidget::tr("query_color"));

	// filter comboBox
	GetComboBoxFilter()->addItem(QWidget::tr("simulation_filter"));
	GetComboBoxFilter()->addItem(QWidget::tr("query_filter"));

	QTimer::singleShot(2000, this, SLOT(OnInitialize()));
}

void MainWindow::connectObject()
{
	// scene tree
	QObject::connect(
		GetSceneTreeView(), SIGNAL(clicked(const QModelIndex)),
		this, SLOT(OnSceneTreeViewClick(const QModelIndex))
	);

	QObject::connect(
		GetSceneTreeView(), SIGNAL(doubleClicked(const QModelIndex)),
		this, SLOT(OnSceneTreeViewClick(const QModelIndex))
	);

	// pick up
	QObject::connect(
		GetGlWidget(), SIGNAL(ShapePicked(physx::PxShape*)),
		this, SLOT(OnShapePicked(physx::PxShape*))
	);

	// menu action
	QObject::connect(
		GetActionOpen(), SIGNAL(triggered()),
		this, SLOT(OnOpenFileFolder())
	);

	QObject::connect(
		GetActionZoomToScene(), SIGNAL(triggered()),
		this, SLOT(OnZoomToScene())
	);

	// camera control
	QObject::connect(
		GetSliderKeyMoveSpeed(), SIGNAL(valueChanged(int)),
		this, SLOT(OnSliderKeyMoveSlide(int))
	);

	QObject::connect(
		GetSliderMouseLeftSpeed(), SIGNAL(valueChanged(int)),
		this, SLOT(OnSliderMouseLeftSlide(int))
	);

	QObject::connect(
		GetSliderMouseRightSpeed(), SIGNAL(valueChanged(int)),
		this, SLOT(OnSliderMouseRightSlide(int))
	);

	QObject::connect(
		GetSliderMouseScrollSpeed(), SIGNAL(valueChanged(int)),
		this, SLOT(OnSliderMouseScrollSlide(int))
	);

	// filter shade
	connect(
		GetComboBoxShade(), SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(OnShadeComboBoxIndexChanged(const QString &))
	);

	connect(
		GetComboBoxFilter(), SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(OnFilterComboBoxIndexChanged(const QString &))
	);

	// word bit op shade
	connect(
		GetPushButtonBitOrWord0(), SIGNAL(clicked()),
		this, SLOT(OnBitOrWord0PushButtonClick())
	);

	connect(
		GetPushButtonBitAndWord0(), SIGNAL(clicked()),
		this, SLOT(OnBitAndWord0PushButtonClick())
	);

	connect(
		GetPushButtonBitOrWord1(), SIGNAL(clicked()),
		this, SLOT(OnBitOrWord1PushButtonClick())
	);

	connect(
		GetPushButtonBitAndWord1(), SIGNAL(clicked()),
		this, SLOT(OnBitAndWord1PushButtonClick())
	);

	connect(
		GetPushButtonBitOrWord2(), SIGNAL(clicked()),
		this, SLOT(OnBitOrWord2PushButtonClick())
	);

	connect(
		GetPushButtonBitAndWord2(), SIGNAL(clicked()),
		this, SLOT(OnBitAndWord2PushButtonClick())
	);

	connect(
		GetPushButtonBitOrWord3(), SIGNAL(clicked()),
		this, SLOT(OnBitOrWord3PushButtonClick())
	);

	connect(
		GetPushButtonBitAndWord3(), SIGNAL(clicked()),
		this, SLOT(OnBitAndWord3PushButtonClick())
	);
}

void MainWindow::showItemAttr(void* ptr, const QString& typeName)
{
	if (!ptr)
	{
		deleteAttrTreeModel();
		return;
	}

	deleteAttrTreeModel();
	m_AttrTreeModel = new TreeModelAttr();
	m_AttrTreeModel->Setup(ptr, typeName);
	GetAttrTreeView()->setModel(m_AttrTreeModel);
	GetAttrTreeView()->expandAll();
}

void MainWindow::showSelectedShape(void* ptr, const QString& typeName)
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}

	std::vector<physx::PxShape*> shapes;

	if (typeName == QString("PxRigidStatic"))
	{
		auto actor = reinterpret_cast<physx::PxRigidStatic*>(ptr);

		auto nbShapes = actor->getNbShapes();
		shapes.resize(nbShapes);
		actor->getShapes(&shapes[0], nbShapes);

		if (shapes.size() > 0)
		{
			auto aabb = { actor->getWorldBounds() };
			scene->GetCamera()->ZoomToBox(CalculateAABB(aabb));
		}
	}
	else if (typeName == QString("PxRigidDynamic"))
	{
		auto actor = reinterpret_cast<physx::PxRigidDynamic*>(ptr);

		auto nbShapes = actor->getNbShapes();
		shapes.resize(nbShapes);
		actor->getShapes(&shapes[0], nbShapes);

		if (shapes.size() > 0)
		{
			auto aabb = { actor->getWorldBounds() };
			scene->GetCamera()->ZoomToBox(CalculateAABB(aabb));
		}
	}
	else if (typeName == QString("PxShape"))
	{
		auto shape = reinterpret_cast<physx::PxShape*>(ptr);
		shapes.push_back(shape);
	}

	scene->SetPickedShapes(shapes);
	GetGlWidget()->update();
}

void MainWindow::deleteAttrTreeModel()
{
	if (m_AttrTreeModel)
	{
		delete m_AttrTreeModel;
		m_AttrTreeModel = nullptr;
	}
}

void MainWindow::deleteSceneTreeModel()
{
	if (m_SceneTreeModel)
	{
		delete m_SceneTreeModel;
		m_AttrTreeModel = nullptr;
	}
}

void MainWindow::bitOpWords(QVector<QString>& wordsStr, BitOpType logicOpType)
{
	auto scene = GetGlWidget()->GetActiveScene();
	if (!scene)
	{
		return;
	}
	std::vector<int> words;
	bool checkStatus;
	for each (auto &wordsStr in wordsStr)
	{
		if (!wordsStr.isEmpty())
		{
			auto word = -1;
			if (wordsStr.contains("0x") or wordsStr.contains("0X"))
			{
				auto hexStr = wordsStr.right(wordsStr.length() - 2);
				word = hexStr.toInt(&checkStatus, 16);
			}
			else
			{
				word = wordsStr.toInt(&checkStatus, 10);
			}
			if (checkStatus)
			{
				words.push_back(word);
			}
			else
			{
				QMessageBox::warning(this, "Warning!", "wrong input");
				return;
			}
		}
		else
		{
			words.push_back(-1);
		}
	}
	scene->ShadeObjectByBitOp(words, logicOpType);
	GetGlWidget()->update();
}
