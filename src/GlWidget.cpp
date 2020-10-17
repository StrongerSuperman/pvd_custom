#include "GlWidget.h"

GlWidget::GlWidget(QWidget* parent) :
    QOpenGLWidget(parent),
	m_Width(1),
	m_Height(1),
	m_ActiveScene(nullptr),
	m_MouseLeftBtnPressed(false),
	m_MouseRightBtnPressed(false)
{
}

GlWidget::~GlWidget()
{
}


void GlWidget::SetScene(Scene *scene)
{
	m_ActiveScene = scene;
	setupCameraView();
}


void GlWidget::initializeGL()
{
	setupCameraView();

	emit Initialized();
}

void GlWidget::resizeGL(int w, int h)
{
	m_Width  = w;
	m_Height = h;
	glViewport(0, 0, m_Width, m_Height);

	setupCameraView();
}

void GlWidget::paintGL()
{
	glViewport(0, 0, m_Width, m_Height);
	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
#ifndef COORDINATE_RIGHT_HANDED
	glFrontFace(GL_CW);
#else
	glFrontFace(GL_CCW);
#endif

	if (m_ActiveScene)
	{
		m_ActiveScene->Render();
	}
}


void GlWidget::mousePressEvent(QMouseEvent *ev)
{
	if (!m_ActiveScene)
	{
		return;
	}

	if (ev->button() == Qt::MouseButton::LeftButton)
	{
		m_ActiveScene->GetCamera()->HandleMouseLeftBtnPress(ev->x(), ev->y());
		m_MouseLeftBtnPressed = true;
	}
	else if (ev->button() == Qt::MouseButton::RightButton)
	{
		if (!m_MouseLeftBtnPressed)
		{
			m_ActiveScene->GetCamera()->HandleMouseRightBtnPress(ev->x(), ev->y());
			m_MouseRightBtnPressed = true;
		}
	}
	update();
}

void GlWidget::mouseMoveEvent(QMouseEvent *ev)
{
	if (!m_ActiveScene)
	{
		return;
	}

	if (m_MouseLeftBtnPressed)
	{
		m_ActiveScene->GetCamera()->HandleMouseLeftBtnMove(ev->x(), ev->y());
	}
	else if (m_MouseRightBtnPressed)
	{
		m_ActiveScene->GetCamera()->HandleMouseRightBtnMove(ev->x(), ev->y());
	}
	update();
}

void GlWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	if (!m_ActiveScene)
	{
		return;
	}

	if (ev->button() == Qt::MouseButton::LeftButton)
	{
		m_ActiveScene->GetCamera()->HandleMouseLeftBtnRelease(ev->x(), ev->y());
		m_MouseLeftBtnPressed = false;
	}
	else if (ev->button() == Qt::MouseButton::RightButton)
	{
		m_ActiveScene->GetCamera()->HandleMouseRightBtnRelease(ev->x(), ev->y());
		m_MouseRightBtnPressed = false;
	}
	update();
}

void GlWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if (!m_ActiveScene)
	{
		return;
	}

	if (ev->button() == Qt::MouseButton::LeftButton)
	{
		m_ActiveScene->GetCamera()->HandleMouseLeftBtnDoubleClick(ev->x(), ev->y());
		emit ShapePicked(m_ActiveScene->OnCameraRayCast());
	}
	update();
}

void GlWidget::wheelEvent(QWheelEvent *ev)
{
	if (!m_ActiveScene)
	{
		return;
	}

	m_ActiveScene->GetCamera()->HandleMouseScroll(ev->delta());
	update();
}

void GlWidget::keyPressEvent(QKeyEvent *ev)
{
	if (!m_ActiveScene)
	{
		return;
	}

	switch (ev->key())
	{
		case Qt::Key_W: m_ActiveScene->GetCamera()->HandleKey('W'); break;
		case Qt::Key_S: m_ActiveScene->GetCamera()->HandleKey('S'); break;
		case Qt::Key_A: m_ActiveScene->GetCamera()->HandleKey('A'); break;
		case Qt::Key_D: m_ActiveScene->GetCamera()->HandleKey('D'); break;
	}
	update();
}

void GlWidget::keyReleaseEvent(QKeyEvent *ev)
{
	if (!m_ActiveScene)
	{
		return;
	}

	(void)(ev);
}

void GlWidget::setupCameraView()
{
	if (!m_ActiveScene)
	{
		return;
	}

	m_ActiveScene->GetCamera()->SetAspectRatio((float)m_Width / (float)m_Height);
	m_ActiveScene->GetCamera()->SetViewPort(0.0f, 0.0f, (float)m_Width, (float)m_Height);
}
