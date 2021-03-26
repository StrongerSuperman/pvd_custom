#pragma once

#include "core/physics/PhysicsEngine.hpp"


class PhysicsObject
{
public:
	physx::PxShape*       shape;
	physx::PxRigidActor*  parentActor;
	physx::PxFilterData   simulationFilterData;
	physx::PxFilterData   queryFilterData;
};
