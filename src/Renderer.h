#pragma once

#include <vector>

#include "RenderProgramPhong.h"


class Renderer
{
public:
	explicit Renderer();
	virtual ~Renderer();

	void Init();
	void Render(std::vector<RenderObject> &objects, Camera &camera);
	void SetPickedRenderObjectIds(const std::vector<int>& ids);

private:
	RenderProgram*          m_RenderProgram;
};
