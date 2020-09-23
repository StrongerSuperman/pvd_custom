#include "Camera.h"


Camera::Camera():
	m_MouseX(0),
	m_MouseY(0),
	m_MouseLeftBtnFirstPress(false),
	m_MouseRightBtnFirstPress(false),
	m_KeyMoveSpeed(200.0f),
	m_MouseLeftSpeed(200.0f),
	m_MouseRightSpeed(100.0f),
	m_MouseScrollSpeed(40.0f),
	m_Ray(Ray())
{
}

Camera::~Camera()
{
}


void Camera::HandleMouseLeftBtnPress(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseLeftBtnFirstPress = true;
}

void Camera::HandleMouseRightBtnPress(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseRightBtnFirstPress = true;
}

void Camera::HandleMouseLeftBtnMove(int x, int y)
{
	if (m_MouseLeftBtnFirstPress)
	{
		m_MouseX = x;
		m_MouseY = y;
		m_MouseLeftBtnFirstPress = false;
		return;
	}

	int dx = m_MouseX - x;
	int dy = y - m_MouseY;
	m_MouseX = x;
	m_MouseY = y;

	auto speed = m_MouseRightSpeed * 0.001f;
	OrbitRotate(dx*speed, -dy* speed);
}

void Camera::HandleMouseRightBtnMove(int x, int y)
{
	if (m_MouseRightBtnFirstPress)
	{
		m_MouseX = x;
		m_MouseY = y;
		m_MouseRightBtnFirstPress = false;
		return;
	}

	int dx = m_MouseX - x;
	int dy = y - m_MouseY;
	m_MouseX = x;
	m_MouseY = y;

	auto speed = m_MouseLeftSpeed * 0.0001f;
	EulerRotate(dx*speed, dy*speed);
}

void Camera::HandleMouseLeftBtnRelease(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseLeftBtnFirstPress = false;
}

void Camera::HandleMouseRightBtnRelease(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseRightBtnFirstPress = false;
}

void Camera::HandleMouseLeftBtnDoubleClick(int x, int y)
{
	updateRay(x, y);
}

void Camera::HandleMouseScroll(int delta)
{
	auto speed = m_MouseScrollSpeed * 0.1f;
	ZoomByMove(delta*speed);
}

void Camera::HandleKey(unsigned char key)
{
	auto speed = m_KeyMoveSpeed * 0.1f;
	switch (toupper(key))
	{
	case 'W':	MoveForward(speed);		break;
	case 'S':	MoveForward(-speed);	break;
	case 'A':	MoveRight(-speed);		break;
	case 'D':	MoveRight(speed);		break;
	}
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

	m_Ray.SetOrigin(GetEye());
	m_Ray.SetDirection(rayDir);
}
