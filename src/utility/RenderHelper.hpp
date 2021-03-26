#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/physics/PhysicsEngine.hpp"
#include "core/render/RenderObject.hpp"
#include "core/render/RenderBuffer.hpp"
#include "core/scene/SceneObject.hpp"
#include "utility/MathHelper.hpp"


# define M_PI 3.14159265358979323846


RenderObject CreateRenderObjectFromPxGeometry(int id, const physx::PxGeometryHolder& geomHd, const physx::PxMat44& posMat, MeshCounter* counter);
RenderBuffer CreateBoxRenderBuffer(float extentX, float extentY, float extentZ);
RenderBuffer CreateCapsuleRenderBuffer(float radius, float halfHeight, uint slices = 10, uint stacks = 10);

RenderObject CreateRenderObjectFromRay(const Ray& ray);
