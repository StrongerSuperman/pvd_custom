#pragma once

#include "CameraFTPS.h"
#include "MathHelper.h"


class Camera : public CameraFTPS
{

public:
	Camera();
	virtual ~Camera();

	void	HandleMouseLeftBtnPress(int x, int y);
	void	HandleMouseRightBtnPress(int x, int y);
	void	HandleMouseLeftBtnMove(int x, int y);
	void	HandleMouseRightBtnMove(int x, int y);
	void	HandleMouseLeftBtnRelease(int x, int y);
	void	HandleMouseRightBtnRelease(int x, int y);
	void    HandleMouseLeftBtnDoubleClick(int x, int y);
	void	HandleMouseScroll(int delta);
	void	HandleKey(unsigned char key);

	inline void SetKeyMoveSpeed(float speed) { m_KeyMoveSpeed = speed; };
	inline void SetMouseLeftSpeed(float speed) { m_MouseLeftSpeed = speed; };
	inline void SetMouseRightSpeed(float speed) { m_MouseRightSpeed = speed; };
	inline void SetMouseScrollSpeed(float speed) { m_MouseScrollSpeed = speed; };

	inline  Ray GetMouseClickRay()  const { return m_Ray; };
			
private:	
	int		m_MouseX;
	int		m_MouseY;
	bool    m_MouseLeftBtnFirstPress;
	bool    m_MouseRightBtnFirstPress;
	float   m_KeyMoveSpeed;
	float   m_MouseLeftSpeed;
	float   m_MouseRightSpeed;
	float   m_MouseScrollSpeed;

	Ray     m_Ray;
	void    updateRay(int x, int y);
};
