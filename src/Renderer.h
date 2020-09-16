#pragma once

#include <vector>

#include "RenderProgramPhongTriangle.h"
#include "RenderProgramPhongLine.h"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init();
	void Render(std::vector<RenderObject> &objects, Camera &camera);
	void RenderLine(std::vector<RenderObject> &objects, Camera &camera);
	void SetPickedRenderObjectIds(const std::vector<int>& ids);

private:
	RenderProgram*          m_RenderProgramPhongTriangle;
	RenderProgram*          m_RenderProgramPhongLine;
};
