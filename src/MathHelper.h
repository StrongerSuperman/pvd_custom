#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <PxPhysics.h>
#include "PxPhysicsAPI.h"
#include <foundation/PxBounds3.h>
#include <foundation/PxMat44.h> 

#include "Ray.h"


glm::mat4x4 PxMat44ToGlmMatrix4x4(const physx::PxMat44 &mat);
glm::vec4 PxVec4ToGlmVector4(const physx::PxVec4 &vec4);
glm::vec3 PxVec3ToGlmVector3(const physx::PxVec3 &vec3);
physx::PxVec4 GlmVector4ToPxVec4(const glm::vec4 &vec4);
physx::PxVec3 GlmVector3ToPxVec3(const glm::vec3 &vec3);
glm::quat PxPxQuatToGlmQuaternion(const physx::PxQuat &quat);
physx::PxBounds3 CalculateAABB(const std::vector<physx::PxBounds3> &bounds);
bool RayCast(const Ray &ray, physx::PxScene* pxScene, physx::PxRaycastBuffer& hitinfo);

