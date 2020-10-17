#include "Camera.h"


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
	float mouseX = x / (GetViewPort().Width  * 0.5f) - 1.0f;
	float mouseY = 1.0f - y / (GetViewPort().Height * 0.5f);
	auto rayClip = glm::vec4(mouseX, mouseY, -1.0f, 1.0f);
	auto rayEye = glm::inverse(GetProjectionMatrix()) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
	auto rayDir = glm::vec3(glm::inverse(GetViewMatrix()) * rayEye);
	rayDir = glm::normalize(rayDir);

#ifndef COORDINATE_RIGHT_HANDED
	m_Ray.SetOrigin(glm::vec3(GetEye().x, GetEye().y, -GetEye().z));
	m_Ray.SetDirection(glm::vec3(rayDir.x, rayDir.y, -rayDir.z));
#else
	m_Ray.SetOrigin(GetEye());
	m_Ray.SetDirection(rayDir);
#endif
}
