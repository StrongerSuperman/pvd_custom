#include "utility/MathHelper.hpp"


Ray::Ray() :
	m_Origin(glm::vec3(0, 0, 0)),
	m_Direction(glm::vec3(0, 0, -1))
{
}

Ray::Ray(const Ray& ray)
{
	m_Origin = ray.GetOrigin();
	m_Direction = ray.GetDirection();
}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
{
	m_Origin = origin;
	m_Direction = direction;
}


MeshCounter::MeshCounter()
{
	Reset();
}

void MeshCounter::Reset()
{
	boxNum = 0;
	sphereNum = 0;
	capsuleNum = 0;
	convexMeshNum = 0;
	triangleMeshNum = 0;
	heightFieldNum = 0;
	planeNum = 0;
}


glm::mat4x4 PxMat44ToGlmMatrix4x4(const physx::PxMat44 &mat)
{
	return glm::mat4x4(
		PxVec4ToGlmVector4(mat.column0),
		PxVec4ToGlmVector4(mat.column1),
		PxVec4ToGlmVector4(mat.column2),
		PxVec4ToGlmVector4(mat.column3));
}

glm::vec4 PxVec4ToGlmVector4(const physx::PxVec4 &vec4)
{
	return glm::vec4(vec4.x, vec4.y, vec4.z, vec4.w);
}

glm::vec3 PxVec3ToGlmVector3(const physx::PxVec3 &vec3)
{
	return glm::vec3(vec3.x, vec3.y, vec3.z);
}

physx::PxVec4 GlmVector4ToPxVec4(const glm::vec4 &vec4)
{
	return physx::PxVec4(vec4.x, vec4.y, vec4.z, vec4.w);
}

physx::PxVec3 GlmVector3ToPxVec3(const glm::vec3 &vec3)
{
	return physx::PxVec3(vec3.x, vec3.y, vec3.z);
}

glm::quat PxPxQuatToGlmQuaternion(const physx::PxQuat &quat)
{
	return glm::quat(quat.w, quat.x, quat.y, quat.z);
}

physx::PxBounds3 CalculateAABB(const std::vector<physx::PxBounds3> &bounds)
{
	std::vector<float> boundsXMax;
	std::vector<float> boundsYMax;
	std::vector<float> boundsZMax;
	std::vector<float> boundsXMin;
	std::vector<float> boundsYMin;
	std::vector<float> boundsZMin;
	for each(auto bound in bounds)
	{
		boundsXMax.push_back(bound.maximum.x);
		boundsYMax.push_back(bound.maximum.y);
		boundsZMax.push_back(bound.maximum.z);
		boundsXMin.push_back(bound.minimum.x);
		boundsYMin.push_back(bound.minimum.y);
		boundsZMin.push_back(bound.minimum.z);
	}
	float xMax = *std::max_element(std::begin(boundsXMax), std::end(boundsXMax));
	float yMax = *std::max_element(std::begin(boundsYMax), std::end(boundsYMax));
	float zMax = *std::max_element(std::begin(boundsZMax), std::end(boundsZMax));
	float xMin = *std::max_element(std::begin(boundsXMin), std::end(boundsXMin));
	float yMin = *std::max_element(std::begin(boundsYMin), std::end(boundsYMin));
	float zMin = *std::max_element(std::begin(boundsZMin), std::end(boundsZMin));

#ifndef COORDINATE_RIGHT_HANDED
	return physx::PxBounds3(
		physx::PxVec3(xMin, yMin, -zMin),
		physx::PxVec3(xMax, yMax, -zMax));
#else
	return physx::PxBounds3(
		physx::PxVec3(xMin, yMin, zMin),
		physx::PxVec3(xMax, yMax, zMax));
#endif
}

bool RayCast(const Ray &ray, physx::PxScene* pxScene, physx::PxRaycastBuffer& hitinfo)
{
	auto origin = GlmVector3ToPxVec3(ray.GetOrigin());
	auto direction = GlmVector3ToPxVec3(ray.GetDirection());
	auto maxDistance = 1000000.0f;

	return pxScene->raycast(origin, direction, maxDistance, hitinfo);
}
