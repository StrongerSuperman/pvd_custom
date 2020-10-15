#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <QDebug>

#include <glm/vec3.hpp>

#include "FileHelper.h"
#include "MathHelper.h"
#include "RenderHelper.h"

#include "IScene.h"

#include "Camera.h"
#include "Renderer.h"
#include "Physics.h"


class Scene: public IScene
{
	using ShapesMap = std::map<physx::PxShape*, SceneObject>;
	using ShapesList = std::vector<physx::PxShape*>;
	using ActorList = std::vector<physx::PxRigidActor*>;

public:
	Scene();
	virtual ~Scene();

	void Load(const std::vector<std::string>& filenames);
	physx::PxBounds3 GetAABB() const;
	virtual void Render() override;

	inline Camera*   GetCamera()   const { return m_Camera; };
	inline Renderer* GetRenderer() const { return m_Renderer; };
	inline Physics*  GetPhysics()  const { return m_Physics; };

	void SetShadeType(ShadeType shadeType);
	void SetFilterType(FilterType filterType);
	void ShadeObjectByBitOp(std::vector<int>& words, BitOpType logicOpType);

	void SetPickedShapeIds(std::vector<int>& ids);
	void SetPickedShapes(ShapesList& shapes);

	inline ActorList  GetActors()  const { return m_Actors;   };

	physx::PxShape* OnCameraRayCast();

private:
	Camera*                      m_Camera;
	Renderer*                    m_Renderer;
	Physics*                     m_Physics;

	MeshCounter*                 m_MeshCounter;
	std::vector<RenderObject>    m_RenderObjectsLine;
	std::vector<int>             m_PickedShapeIds;

	ActorList                    m_Actors;
	ShapesMap                    m_ShapeMap;

	ShadeType                    m_ShadeType;
	FilterType                   m_FilterType;

	void shadeObjectByShadeType();
	void shadeObjectByBitOpType(std::vector<int>& words, BitOpType logicOpType);

	void genRenderObjectRay(const Ray& ray);
	void pintMeshCounter();
};
