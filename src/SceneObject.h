#pragma once

#include "RenderObject.h"
#include "physicsObject.h"


class SceneObject
{
public:
	int            id;
	RenderObject   renderData;
	PhysicsObject  physicsData;
};
