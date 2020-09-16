#pragma once

#include "RenderProgramPhong.h"


class RenderProgramPhongTriangle : public RenderProgramPhong
{
public:
	explicit RenderProgramPhongTriangle();
	virtual ~RenderProgramPhongTriangle();

	void Render(const RenderObject& object, const Camera& camera) override;

protected:
	void SetVetexAttrEnable(bool enable);
};
