#pragma once

#include "ICamera.h"
#include "MathHelper.h"


class Camera : public ICamera
{

public:
	Camera();
	virtual ~Camera();
	void ZoomToBox(physx::PxBounds3 aabb);

	void HandleMouseLeftBtnDoubleClick(int x, int y) override;
	inline  Ray GetMouseClickRay()  const { return m_Ray; };
			
private:
	Ray     m_Ray;
	void    updateRay(int x, int y);
};
