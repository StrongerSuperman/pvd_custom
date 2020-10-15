#pragma once

#include "physicsEngine.h"


class PhysicsObject
{
public:
	physx::PxShape*       shape;
	physx::PxRigidActor*  parentActor;
	physx::PxFilterData   simulationFilterData;
	physx::PxFilterData   queryFilterData;
};
