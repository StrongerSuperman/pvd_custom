#include "Camera.h"


Camera::Camera():
	m_MouseX(0),
	m_MouseY(0),
	m_MouseLeftBtnFirstPress(false),
	m_MouseRightBtnFirstPress(false),
	m_KeyMoveSpeed(20.0f),
	m_MouseMoveSpeed(0.02f),
	m_MouseRotateSpeed(0.1f),
	m_MouseScrollSpeed(4.0f),
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

	OrbitRotate(dx*m_MouseRotateSpeed, -dy* m_MouseRotateSpeed);
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

	EulerRotate(dx*m_MouseMoveSpeed, dy*m_MouseMoveSpeed);
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
	ZoomByMove(delta*m_MouseScrollSpeed);
}

void Camera::HandleKey(unsigned char key)
{
	switch (toupper(key))
	{
	case 'W':	MoveForward(m_KeyMoveSpeed);		break;
	case 'S':	MoveForward(-m_KeyMoveSpeed);		break;
	case 'A':	MoveRight(-m_KeyMoveSpeed);		    break;
	case 'D':	MoveRight(m_KeyMoveSpeed);		    break;
	}
}


void Camera::updateRay(int x, int y)
{
	float mouseX = x / (GetViewPort().Width  * 0.5f) - 1.0f;
	float mouseY = 1.0f - y / (GetViewPort().Height * 0.5f);
	auto invVP = glm::inverse(GetProjectionViewMatrix());

	glm::vec4 screenPos = glm::vec4(mouseX, mouseY, 1.0f, 1.0f);
	glm::vec4 worldPos = invVP * screenPos;
	glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

	m_Ray.SetOrigin(GetEye());
	m_Ray.SetDirection(dir);
}
