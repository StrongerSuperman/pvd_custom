#include "logic/Camera.hpp"


Camera::Camera():
	m_Ray(Ray())
{
}

Camera::~Camera()
{
}

void Camera::ZoomToBox(physx::PxBounds3 aabb)
{
	glm::vec3 center = PxVec3ToGlmVector3(aabb.getCenter());
	glm::vec3 minimum = PxVec3ToGlmVector3(aabb.minimum);
	glm::vec3 maximum = PxVec3ToGlmVector3(aabb.maximum);
	glm::vec3 eye = glm::vec3(center.x, center.y, maximum.z + 3 * (maximum.z - minimum.z));

	SetEyeAndTarget(eye, center);
}

void Camera::HandleMouseLeftBtnDoubleClick(int x, int y)
{
	updateRay(x, y);
}


void Camera::updateRay(int x, int y)
{
	// code reference website: https://antongerdelan.net/opengl/raycasting.html
	float ndcX = 2.0 * x / GetViewPort().Width - 1.0f;
	float ndcY = 1.0f - 2.0 * y / GetViewPort().Height;
	auto rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
	auto rayView = glm::inverse(GetProjectionMatrix()) * rayClip;
	rayView = glm::vec4(rayView.x, rayView.y, -1.0, 0.0);
	auto rayWorld = glm::vec3(glm::inverse(GetViewMatrix()) * rayView);
	auto rayDir = glm::normalize(rayWorld);

#ifndef COORDINATE_RIGHT_HANDED
	m_Ray.SetOrigin(glm::vec3(GetEye().x, GetEye().y, -GetEye().z));
	m_Ray.SetDirection(glm::vec3(rayDir.x, rayDir.y, -rayDir.z));
#else
	m_Ray.SetOrigin(GetEye());
	m_Ray.SetDirection(rayDir);
#endif
}
