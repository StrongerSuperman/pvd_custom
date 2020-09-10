#pragma once

#include <iostream>
#include <algorithm>

#include "CameraBase.h"


class CameraFTPS : public CameraBase
{
public:
	CameraFTPS();
	virtual ~CameraFTPS();

	inline void  SetRotateRadius(float radius) { m_RotateRadius = radius; };
	inline float GetRotateRadius() const { return m_RotateRadius; };
					       
protected:
	void MoveForward(float delta);
	void MoveRight(float delta);
	void ZoomByFov(float delta);
	void ZoomByMove(float delta);
	void EulerRotate(float deltaX, float deltaY);
	void OrbitRotate(float deltaX, float deltaY);

private:
	float m_Yaw;
	float m_Pitch;
	float m_RotateRadius;

	glm::vec3 rotatePointByVector(const glm::vec3& p, const glm::vec3& v, float theta);
};
