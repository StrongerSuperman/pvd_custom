#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicsEngine.h"
#include "RenderObject.h"
#include "RenderBuffer.h"
#include "SceneObject.h"
#include "MathHelper.h"


# define M_PI 3.14159265358979323846


RenderObject CreateRenderObjectFromPxGeometry(int id, const physx::PxGeometryHolder& geomHd, const physx::PxMat44& posMat, MeshCounter* counter);
RenderBuffer CreateBoxRenderBuffer(float extentX, float extentY, float extentZ);
RenderBuffer CreateCapsuleRenderBuffer(float radius, float halfHeight, uint slices = 10, uint stacks = 10);

RenderObject CreateRenderObjectFromRay(const Ray& ray);
