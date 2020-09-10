#include "CameraFTPS.h"


CameraFTPS::CameraFTPS():
	m_Yaw(0.0f),
	m_Pitch(0.0f),
	m_RotateRadius(0.0f)
{
}

CameraFTPS::~CameraFTPS()
{
}


void CameraFTPS::MoveForward(float delta)
{
	auto eye = GetEye();
	auto dir = GetDir();

	eye = eye + delta * dir;
	SetEye(eye);

	UpdateViewMatrix();
}

void CameraFTPS::MoveRight(float delta)
{
	auto eye = GetEye();
	auto dir = GetDir();

	auto right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
	eye = eye + delta * right;
	SetEye(eye);

	UpdateViewMatrix();
}

void CameraFTPS::ZoomByFov(float delta)
{
	float fov = GetFov();

	fov += delta;
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
	else if (fov > 150.0f)
	{
		fov = 150.0f;
	}

	SetFov(fov);
	UpdateProjectionMatrix();
}

void CameraFTPS::ZoomByMove(float delta)
{
	auto eye = GetEye();
	auto dir = GetDir();

	glm::vec3 center = eye + m_RotateRadius * dir;
	eye += (float)delta * dir;
	m_RotateRadius = glm::distance(center, eye);

	SetEye(eye);
	UpdateViewMatrix();
}

void CameraFTPS::EulerRotate(float deltaX, float deltaY)
{
	auto dir = GetDir();

	glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
	glm::vec3 up = glm::normalize(glm::cross(right, dir));

	float yaw = glm::radians(deltaX);
	float pitch = glm::radians(deltaY);

	auto qx = glm::quat(yaw, up);
	auto qy = glm::quat(pitch, right);
	dir = glm::normalize(qy * qx * dir);

	SetDir(dir);
	UpdateViewMatrix();
}

void CameraFTPS::OrbitRotate(float deltaX, float deltaY)
{
	auto eye = GetEye();
	auto dir = GetDir();

	glm::vec3 center = eye + m_RotateRadius * dir;
	glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0)));
	glm::vec3 up = glm::normalize(glm::cross(right, dir));

	glm::vec3 tmpEye = eye - center;

	m_Yaw += deltaX;
	tmpEye = rotatePointByVector(tmpEye, up, deltaX);

	m_Pitch += deltaY;
	if (-80.0f < m_Pitch && m_Pitch < 80.0f)
	{
		tmpEye = rotatePointByVector(tmpEye, right, deltaY);
	}
	else
	{
		m_Pitch = std::min(m_Pitch, 80.0f);
		m_Pitch = std::max(m_Pitch, -80.0f);
	}

	eye = tmpEye + center;
	dir = glm::normalize(center - eye);

	SetEye(eye);
	SetDir(dir);
	UpdateViewMatrix();
}


glm::vec3 CameraFTPS::rotatePointByVector(const glm::vec3& p, const glm::vec3& v, float theta)
{
	float r = glm::radians(theta);
	float c = cos(r);
	float s = sin(r);
	float newX = (v.x*v.x*(1 - c) + c) * p.x + (v.x*v.y*(1 - c) - v.z * s) * p.y + (v.x*v.z*(1 - c) + v.y * s) * p.z;
	float newY = (v.y*v.x*(1 - c) + v.z * s) * p.x + (v.y*v.y*(1 - c) + c) * p.y + (v.y*v.z*(1 - c) - v.x * s) * p.z;
	float newZ = (v.x*v.z*(1 - c) - v.y * s) * p.x + (v.y*v.z*(1 - c) + v.x * s) * p.y + (v.z*v.z*(1 - c) + c) * p.z;
	return glm::vec3(newX, newY, newZ);
}
