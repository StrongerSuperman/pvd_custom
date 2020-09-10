#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <foundation/PxBounds3.h>
#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>

#include "PhysicsScene.h"
#include "PhysxHelper.h"


using shapesMap = std::map<physx::PxShape*, std::pair<uint, physx::PxRigidActor*>>;

class Scene
{
public:
	explicit Scene();
	virtual ~Scene();

	void Load(std::vector<std::string> filenames);
	physx::PxBounds3 GetAABB() const;

	inline std::vector<physx::PxRigidActor*>      GetActors()       const { return m_Actors;       };
	inline shapesMap                              GetShapesMap()    const { return m_PxShapesMap;  };
	inline PhysicsScene*                          GetPhysicsScene() const { return m_PhysicsScene; };

private:
	std::vector<physx::PxRigidActor*>  m_Actors;
	shapesMap                          m_PxShapesMap;
	PhysicsScene*                      m_PhysicsScene;
};
