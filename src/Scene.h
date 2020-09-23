#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <QDebug>

#include <foundation/PxBounds3.h>
#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>

#include <glm/vec3.hpp>

#include "Camera.h"
#include "Renderer.h"
#include "RenderObject.h"
#include "RenderBuffer.h"
#include "physicsWorld.h"
#include "FileHelper.h"
#include "MathHelper.h"
#include "RenderHelper.h"


class Scene
{
	using shapesMap = std::map<physx::PxShape*, std::pair<int, physx::PxRigidActor*>>;
	using shapesList = std::vector<physx::PxShape*>;
	using ActorList = std::vector<physx::PxRigidActor*>;

public:
	Scene();
	~Scene();

	void Load(const std::vector<std::string>& filenames);
	physx::PxBounds3 GetAABB() const;

	void Render();

	void ResetCamera();
	physx::PxShape* OnCameraRayCast();

	void SetPickedShapeIds(std::vector<int>& ids);
	void SetPickedShapes(shapesList& shapes);

	inline Camera*         GetCamera()       const { return m_Camera;       };
	inline Renderer*       GetRenderer()     const { return m_Renderer;     };
	inline PhysicsWorld*   GetPhysicsWorld() const { return m_PhysicsWorld; };
	inline ActorList       GetActors()       const { return m_Actors;       };
	inline shapesMap       GetShapesMap()    const { return m_PxShapesMap;  };

private:
	Camera*                      m_Camera;
	Renderer*                    m_Renderer;
	PhysicsWorld*                m_PhysicsWorld;

	MeshCounter*                 m_MeshCounter;
	std::vector<RenderObject>    m_RenderObjects;
	std::vector<RenderObject>    m_RenderObjectsLine;
	std::vector<int>             m_PickedShapeIds;

	ActorList                    m_Actors;
	shapesMap                    m_PxShapesMap;

	void createRenderObjects();
	void genRenderObjectRay(const Ray& ray);
	void pintMeshCounter();
};
