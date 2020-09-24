#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PhysicsEngine.h"


class Ray
{
public:
	Ray();
	Ray(const Ray& ray);
	Ray(const glm::vec3& origin, const glm::vec3& direction);

	inline void SetOrigin(const glm::vec3& origin) { m_Origin = origin; };
	inline void SetDirection(const glm::vec3& direction) { m_Direction = direction; };

	inline glm::vec3 GetOrigin() const { return m_Origin; };
	inline glm::vec3 GetDirection() const { return m_Direction; };

private:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
};


class MeshCounter
{
public:
	explicit MeshCounter();

	void Reset();

	int sphereNum;
	int planeNum;
	int capsuleNum;
	int boxNum;
	int convexMeshNum;
	int triangleMeshNum;
	int heightFieldNum;
};


glm::mat4x4 PxMat44ToGlmMatrix4x4(const physx::PxMat44 &mat);
glm::vec4 PxVec4ToGlmVector4(const physx::PxVec4 &vec4);
glm::vec3 PxVec3ToGlmVector3(const physx::PxVec3 &vec3);
physx::PxVec4 GlmVector4ToPxVec4(const glm::vec4 &vec4);
physx::PxVec3 GlmVector3ToPxVec3(const glm::vec3 &vec3);
glm::quat PxPxQuatToGlmQuaternion(const physx::PxQuat &quat);
physx::PxBounds3 CalculateAABB(const std::vector<physx::PxBounds3> &bounds);
bool RayCast(const Ray &ray, physx::PxScene* pxScene, physx::PxRaycastBuffer& hitinfo);

