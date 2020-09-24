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
#include "PhysicsWorld.h"
#include "SceneObject.h"
#include "FileHelper.h"
#include "MathHelper.h"
#include "RenderHelper.h"


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

	void ResetCamera();
	physx::PxShape* OnCameraRayCast();

	void SetPickedShapeIds(std::vector<int>& ids);
	void SetPickedShapes(ShapesList& shapes);

	inline Camera*         GetCamera()      const { return m_Camera;   };
	inline Renderer*       GetRenderer()    const { return m_Renderer; };
	inline PhysicsWorld*   GetPhysics()     const { return m_Physics;  };

	inline ActorList       GetActors()      const { return m_Actors;   };

private:
	Camera*                      m_Camera;
	Renderer*                    m_Renderer;
	PhysicsWorld*                m_Physics;

	MeshCounter*                 m_MeshCounter;
	std::vector<SceneObject>     m_SceneObjects;
	std::vector<RenderObject>    m_RenderObjectsLine;
	std::vector<int>             m_PickedShapeIds;

	ActorList                    m_Actors;
	ShapesMap                    m_ShapeMap;

	void genRenderObjectRay(const Ray& ray);
	void pintMeshCounter();
};
