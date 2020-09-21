#pragma once

#include <vector>

#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLWidget>

#include <glm/vec3.hpp>

#include "Camera.h"
#include "Renderer.h"
#include "RenderObject.h"
#include "RenderBuffer.h"
#include "MathHelper.h"
#include "RenderHelper.h"

#include "Scene.h"


class GlWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit GlWidget(QWidget *parent = NULL);
	~GlWidget();

	void    SetScene(Scene *scene);
	void    ResetCamera();
	void    SetPickedShapeIds(std::vector<int>& ids);
	void    SetPickedShapes(std::vector<physx::PxShape*>& shapes);

	inline  Scene*   GetActiveScene() const    { return m_ActiveScene;   };
	inline  Camera*  GetCamera()      const    { return m_Camera;        };

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
	int                                 m_Width;
	int                                 m_Height;

	Camera*                             m_Camera;
	Renderer*                           m_Renderer;
	Scene*                              m_ActiveScene;
	MeshCounter*                        m_MeshCounter;

	std::vector<RenderObject>           m_RenderObjects;
	std::vector<RenderObject>           m_RenderObjectsLine;
	std::vector<int>                    m_PickedShapeIds;
						                
	bool                                m_MouseLeftBtnPressed;
	bool                                m_MouseRightBtnPressed;

	void createRenderObjects();
	void onCameraRayCast();
	void genRenderObjectRay(const Ray& ray);

	void pintMeshCounter();
};
