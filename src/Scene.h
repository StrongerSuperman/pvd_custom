#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <foundation/PxBounds3.h>
#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>

#include "physicsWorld.h"
#include "PhysxHelper.h"


using shapesMap = std::map<physx::PxShape*, std::pair<uint, physx::PxRigidActor*>>;

class Scene
{
public:
	Scene();
	~Scene();

	void Load(std::vector<std::string> filenames);
	physx::PxBounds3 GetAABB() const;

	inline std::vector<physx::PxRigidActor*>      GetActors()       const { return m_Actors;       };
	inline shapesMap                              GetShapesMap()    const { return m_PxShapesMap;  };
	inline PhysicsWorld*                          GetPhysicsWorld() const { return m_PhysicsWorld; };

private:
	std::vector<physx::PxRigidActor*>  m_Actors;
	shapesMap                          m_PxShapesMap;
	PhysicsWorld*                      m_PhysicsWorld;
};
