#include "GlWidget.h"

GlWidget::GlWidget(QWidget* parent) :
    QOpenGLWidget(parent),
	m_Width(1),
	m_Height(1),
	m_Camera(new Camera),
	m_Renderer(new Renderer),
	m_PickedShapeIds({}),
	m_ActiveSceneIdx(-1),
	m_ActiveScene(nullptr),
	m_MouseLeftBtnPressed(false),
	m_MouseRightBtnPressed(false)
{
}

GlWidget::~GlWidget()
{
	delete m_Camera;
	delete m_Renderer;
}


void GlWidget::ActiveScene(int sceneIdx)
{
	Q_ASSERT(m_Scenes.size() > sceneIdx && m_Scenes[sceneIdx] != NULL);

	m_ActiveSceneIdx = sceneIdx;
	m_ActiveScene = m_Scenes[sceneIdx];

	ResetCamera();
}

void GlWidget::AddScene(Scene *scene)
{
	m_Scenes.push_back(scene);
}


void GlWidget::ResetCamera()
{
	physx::PxBounds3 aabb = m_ActiveScene->GetAABB();
	glm::vec3 center = PhysxHelper::PxVec3ToGlmVector3(aabb.getCenter());
	glm::vec3 minimum = PhysxHelper::PxVec3ToGlmVector3(aabb.minimum);
	glm::vec3 maximum = PhysxHelper::PxVec3ToGlmVector3(aabb.maximum);
	glm::vec3 eye = glm::vec3(center.x, center.y, maximum.z + 3 * (maximum.z - minimum.z));
	glm::vec3 dir = glm::normalize(center - eye);

	m_Camera->SetEyeAndDir(eye, dir);
	m_Camera->SetOrthgonalData(minimum, maximum);
	m_Camera->SetRotateRadius(glm::distance(eye, center));
}


void GlWidget::SetPickedShapeIds(std::vector<int>& ids)
{ 
	m_PickedShapeIds = ids; 
	m_Renderer->SetPickedRenderObjectIds(ids); 
}

void GlWidget::SetPickedShapes(std::vector<physx::PxShape*>& shapes) 
{
	std::vector<int> ids;
	for each (auto &shape in shapes)
	{
		ids.push_back(m_ActiveScene->GetShapesMap()[shape].first);
	}
	SetPickedShapeIds(ids);
}


void GlWidget::initializeGL()
{
	m_Renderer->Init();

	createRenderObjects();

	m_Camera->SetAspectRatio((float)m_Width / (float)m_Height);
	m_Camera->SetViewPort(0.0f, 0.0f, (float)m_Width, (float)m_Height);

	emit Initialized();
}

void GlWidget::resizeGL(int w, int h)
{
	m_Width  = w;
	m_Height = h;
	glViewport(0, 0, m_Width, m_Height);

	m_Camera->SetAspectRatio((float)m_Width / (float)m_Height);
	m_Camera->SetViewPort(0.0f, 0.0f, (float)m_Width, (float)m_Height);
}

void GlWidget::paintGL()
{
	glViewport(0, 0, m_Width, m_Height);
	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_Renderer->Render(m_RenderObjects, *m_Camera);

	QTimer::singleShot(1000 / 60, this, SLOT(update()));
}


void GlWidget::mousePressEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::MouseButton::LeftButton)
	{
		m_Camera->HandleMouseLeftBtnPress(ev->x(), ev->y());
		m_MouseLeftBtnPressed = true;
	}
	else if (ev->button() == Qt::MouseButton::RightButton)
	{
		if (!m_MouseLeftBtnPressed)
		{
			m_Camera->HandleMouseRightBtnPress(ev->x(), ev->y());
			m_MouseRightBtnPressed = true;
		}
	}
}

void GlWidget::mouseMoveEvent(QMouseEvent *ev)
{
	if (m_MouseLeftBtnPressed)
	{
		m_Camera->HandleMouseLeftBtnMove(ev->x(), ev->y());
	}
	else if (m_MouseRightBtnPressed)
	{
		m_Camera->HandleMouseRightBtnMove(ev->x(), ev->y());
	}
}

void GlWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::MouseButton::LeftButton)
	{
		m_Camera->HandleMouseLeftBtnRelease(ev->x(), ev->y());
		m_MouseLeftBtnPressed = false;
	}
	else if (ev->button() == Qt::MouseButton::RightButton)
	{
		m_Camera->HandleMouseRightBtnRelease(ev->x(), ev->y());
		m_MouseRightBtnPressed = false;
	}
}

void GlWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::MouseButton::LeftButton)
	{
		m_Camera->HandleMouseLeftBtnDoubleClick(ev->x(), ev->y());
		onCameraRayCast();
	}
}

void GlWidget::wheelEvent(QWheelEvent *ev)
{
	m_Camera->HandleMouseScroll(ev->delta());
}

void GlWidget::keyPressEvent(QKeyEvent *ev)
{
	switch (ev->key())
	{
		case Qt::Key_W: m_Camera->HandleKey('W'); break;
		case Qt::Key_S: m_Camera->HandleKey('S'); break;
		case Qt::Key_A: m_Camera->HandleKey('A'); break;
		case Qt::Key_D: m_Camera->HandleKey('D'); break;
	}
}

void GlWidget::keyReleaseEvent(QKeyEvent *ev)
{
	(void)(ev);
}


void GlWidget::createRenderObjects()
{
	m_RenderObjects.clear();
	PhysxHelper::CreateRenderObjectFromShapes(m_RenderObjects, m_ActiveScene->GetShapesMap());
}

void GlWidget::onCameraRayCast()
{
	if (!m_ActiveScene)
	{
		return;
	}

	physx::PxRaycastBuffer hitinfo;
	auto isHit = PhysxHelper::RayCast(m_Camera->GetMouseClickRay(), m_ActiveScene->GetPhysicsScene()->GetPxScene(), hitinfo);
	if (isHit)
	{
		m_PickedShapeIds = { static_cast<int>(m_ActiveScene->GetShapesMap()[hitinfo.block.shape].first) };
	}
	else
	{
		m_PickedShapeIds = {};
	}
	SetPickedShapeIds(m_PickedShapeIds);
	emit ShapePicked(hitinfo.block.shape);
}
