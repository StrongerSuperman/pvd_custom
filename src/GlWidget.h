#pragma once

#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLWidget>

#include "Scene.h"


class GlWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit GlWidget(QWidget *parent = NULL);
	~GlWidget();

	void    SetScene(Scene *scene);
	inline  Scene*   GetActiveScene() const    { return m_ActiveScene; };

signals:
	void Initialized();
	void ShapePicked(physx::PxShape*);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void mouseDoubleClickEvent(QMouseEvent *ev) override;
	void wheelEvent(QWheelEvent *ev) override;
	void keyPressEvent(QKeyEvent *ev) override;
	void keyReleaseEvent(QKeyEvent *ev) override;

private:
	int      m_Width;
	int      m_Height;
	
	Scene*   m_ActiveScene;

	bool     m_MouseLeftBtnPressed;
	bool     m_MouseRightBtnPressed;
};
