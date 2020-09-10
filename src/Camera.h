#pragma once

#include "CameraFTPS.h"


class Ray
{
public:
	Ray();
	Ray(const Ray& ray);
	Ray(const glm::vec3& origin, const glm::vec3& direction);

	inline void SetOrigin(const glm::vec3& origin)       { m_Origin = origin;       };
	inline void SetDirection(const glm::vec3& direction) { m_Direction = direction; };

	inline glm::vec3 GetOrigin() const    { return m_Origin;    };
	inline glm::vec3 GetDirection() const { return m_Direction; };

private:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
};

class Camera : public CameraFTPS
{

public:
	Camera();
	~Camera();

	void	HandleMouseLeftBtnPress(int x, int y);
	void	HandleMouseRightBtnPress(int x, int y);
	void	HandleMouseLeftBtnMove(int x, int y);
	void	HandleMouseRightBtnMove(int x, int y);
	void	HandleMouseLeftBtnRelease(int x, int y);
	void	HandleMouseRightBtnRelease(int x, int y);
	void    HandleMouseLeftBtnDoubleClick(int x, int y);
	void	HandleMouseScroll(int delta);
	void	HandleKey(unsigned char key);

	inline  Ray GetMouseClickRay()  const { return m_Ray; };
			
private:	
	int		m_MouseX;
	int		m_MouseY;
	bool    m_MouseLeftBtnFirstPress;
	bool    m_MouseRightBtnFirstPress;
	float   m_MouseMoveSpeed;
	float   m_MouseRotateSpeed;
	float   m_MouseScrollSpeed;
	float   m_KeyMoveSpeed;

	Ray     m_Ray;
	void    updateRay(int x, int y);
};
