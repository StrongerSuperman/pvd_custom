#pragma once

#include "RenderObject.h"
#include "physicsWorld.h"


class PhysicsData
{
public:
	physx::PxShape*       shape;
	physx::PxRigidActor*  parentActor;
	physx::PxFilterData   simulationFilterData;
	physx::PxFilterData   queryFilterData;
};


class SceneObject
{
public:
	int            id;
	RenderObject   renderData;
	PhysicsData    physicsData;
};
