#pragma once

#include <vector>

#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>

#include <QOpenGLWidget>

#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "RenderObject.h"


class GlWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit GlWidget(QWidget *parent = NULL);
	virtual ~GlWidget();

	void    ActiveScene(int sceneIdx);
	void    AddScene(Scene *scene);

	void    ResetCamera();
	void    SetPickedShapeId(int id);

	inline  int       GetPickedShapeId()        { return m_PickedShapeId; };
	inline  Scene*    GetActiveScene() const    { return m_ActiveScene;   };
	inline  Camera*   GetCamera()      const    { return m_Camera;        };

signals:
	void Initialized();

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	virtual void mousePressEvent(QMouseEvent *ev);
	virtual void mouseMoveEvent(QMouseEvent *ev);
	virtual void mouseReleaseEvent(QMouseEvent *ev);
	virtual void mouseDoubleClickEvent(QMouseEvent *ev);
	virtual void wheelEvent(QWheelEvent *ev);
	virtual void keyPressEvent(QKeyEvent *ev);
	virtual void keyReleaseEvent(QKeyEvent *ev);

private:
	int                                 m_Width;
	int                                 m_Height;

	Camera*                             m_Camera;
	Renderer*                           m_Renderer;
	std::vector<RenderObject>           m_RenderObjects;

	int                                 m_PickedShapeId;

	QVector<Scene*>                     m_Scenes;
	int                                 m_ActiveSceneIdx;
	Scene*                              m_ActiveScene;
						                
	bool                                m_MouseLeftBtnPressed;
	bool                                m_MouseRightBtnPressed;

	void createRenderObjects();
	void onCameraRayCast();
};
