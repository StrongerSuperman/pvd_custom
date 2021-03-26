#pragma once

#include "core/camera/CameraFTPS.hpp"


class ICamera : public CameraFTPS
{

public:
	ICamera();
	virtual ~ICamera();

	virtual void HandleMouseLeftBtnPress(int x, int y);
	virtual void HandleMouseRightBtnPress(int x, int y);
	virtual void HandleMouseLeftBtnMove(int x, int y);
	virtual void HandleMouseRightBtnMove(int x, int y);
	virtual void HandleMouseLeftBtnRelease(int x, int y);
	virtual void HandleMouseRightBtnRelease(int x, int y);
	virtual void HandleMouseLeftBtnDoubleClick(int x, int y);
	virtual void HandleMouseScroll(int delta);
	virtual void HandleKey(unsigned char key);

	inline void SetKeyMoveSpeed(float speed) { m_KeyMoveSpeed = speed; };
	inline void SetMouseLeftSpeed(float speed) { m_MouseLeftSpeed = speed; };
	inline void SetMouseRightSpeed(float speed) { m_MouseRightSpeed = speed; };
	inline void SetMouseScrollSpeed(float speed) { m_MouseScrollSpeed = speed; };

private:	
	int		m_MouseX;
	int		m_MouseY;
	bool    m_MouseLeftBtnFirstPress;
	bool    m_MouseRightBtnFirstPress;
	float   m_KeyMoveSpeed;
	float   m_MouseLeftSpeed;
	float   m_MouseRightSpeed;
	float   m_MouseScrollSpeed;
};
