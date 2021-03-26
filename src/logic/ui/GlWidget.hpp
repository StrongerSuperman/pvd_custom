#pragma once

#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLWidget>

#include "logic/Scene.hpp"


class GlWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit GlWidget(QWidget *parent = NULL);
	virtual ~GlWidget();

	void    SetScene(Scene *scene);
	inline  Scene*  GetActiveScene() const { return m_ActiveScene; };

signals:
	void Initialized();
	void ShapePicked(physx::PxShape*);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	virtual void mousePressEvent(QMouseEvent *ev) override;
	virtual void mouseMoveEvent(QMouseEvent *ev) override;
	virtual void mouseReleaseEvent(QMouseEvent *ev) override;
	void mouseDoubleClickEvent(QMouseEvent *ev) override;
	void wheelEvent(QWheelEvent *ev) override;
	void keyPressEvent(QKeyEvent *ev) override;
	void keyReleaseEvent(QKeyEvent *ev) override;

	void setupCameraView();

	Scene*  m_ActiveScene;
	int     m_Width;
	int     m_Height;
	bool    m_MouseLeftBtnPressed;
	bool    m_MouseRightBtnPressed;
};
