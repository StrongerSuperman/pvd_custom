#pragma once

#include "RenderProgramPhong.h"


class RenderProgramPhongLine : public RenderProgramPhong
{
public:
	explicit RenderProgramPhongLine();
	virtual ~RenderProgramPhongLine();

	void Render(const RenderObject& object, const Camera& camera) override;

protected:
	void SetVetexAttrEnable(bool enable);
};
