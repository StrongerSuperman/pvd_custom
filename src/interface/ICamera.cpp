#include "interface/ICamera.hpp"


ICamera::ICamera():
	m_MouseX(0),
	m_MouseY(0),
	m_MouseLeftBtnFirstPress(false),
	m_MouseRightBtnFirstPress(false),
	m_KeyMoveSpeed(200.0f),
	m_MouseLeftSpeed(200.0f),
	m_MouseRightSpeed(100.0f),
	m_MouseScrollSpeed(40.0f)
{
}

ICamera::~ICamera()
{
}


void ICamera::HandleMouseLeftBtnPress(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseLeftBtnFirstPress = true;
}

void ICamera::HandleMouseRightBtnPress(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseRightBtnFirstPress = true;
}

void ICamera::HandleMouseLeftBtnMove(int x, int y)
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

void ICamera::HandleMouseRightBtnMove(int x, int y)
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

void ICamera::HandleMouseLeftBtnRelease(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseLeftBtnFirstPress = false;
}

void ICamera::HandleMouseRightBtnRelease(int x, int y)
{
	(void)(x);
	(void)(y);
	m_MouseRightBtnFirstPress = false;
}

void ICamera::HandleMouseLeftBtnDoubleClick(int x, int y)
{
	(void)(x);
	(void)(y);
}

void ICamera::HandleMouseScroll(int delta)
{
	auto speed = m_MouseScrollSpeed * 0.1f;
	ZoomByMove(delta*speed);
}

void ICamera::HandleKey(unsigned char key)
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
