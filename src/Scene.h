#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <QDebug>

#include <glm/vec3.hpp>

#include "RenderObject.h"
#include "RenderBuffer.h"
#include "SceneObject.h"

#include "FileHelper.h"
#include "MathHelper.h"
#include "RenderHelper.h"

#include "Camera.h"
#include "Renderer.h"
#include "Physics.h"


class Scene
{
	using ShapesMap = std::map<physx::PxShape*, SceneObject>;
	using ShapesList = std::vector<physx::PxShape*>;
	using ActorList = std::vector<physx::PxRigidActor*>;

public:
	Scene();
	~Scene();

	void Load(const std::vector<std::string>& filenames);
	physx::PxBounds3 GetAABB() const;
	void Render();

	void SetShadeType(ShadeType shadeType);
	void SetFilterType(FilterType filterType);
	void ShadeObjectByLogicOp(std::vector<int>& words, LogicOpType logicOpType);

	void SetPickedShapeIds(std::vector<int>& ids);
	void SetPickedShapes(ShapesList& shapes);

	inline Camera*         GetCamera()      const { return m_Camera;   };
	inline Renderer*       GetRenderer()    const { return m_Renderer; };
	inline Physics*        GetPhysics()     const { return m_Physics;  };
	inline ActorList       GetActors()      const { return m_Actors;   };

	physx::PxShape* OnCameraRayCast();

private:
	Camera*                      m_Camera;
	Renderer*                    m_Renderer;
	Physics*                     m_Physics;

	MeshCounter*                 m_MeshCounter;
	std::vector<SceneObject>     m_SceneObjects;
	std::vector<RenderObject>    m_RenderObjectsLine;
	std::vector<int>             m_PickedShapeIds;

	ActorList                    m_Actors;
	ShapesMap                    m_ShapeMap;

	ShadeType                    m_ShadeType;
	FilterType                   m_FilterType;

	void shadeObjectByShadeType();
	void shadeObjectByLogicOpType(std::vector<int>& words, LogicOpType logicOpType);

	void genRenderObjectRay(const Ray& ray);
	void pintMeshCounter();
};
