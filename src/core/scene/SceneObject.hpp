#pragma once

#include "core/render/RenderObject.hpp"
#include "core/physics/PhysicsObject.hpp"


class SceneObject
{
public:
	int            id;
	RenderObject   renderData;
	PhysicsObject  physicsData;
};
