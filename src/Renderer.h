#pragma once

#include <vector>

#include "RenderProgramPhong.h"
#include "RenderProgramLine.h"


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
	RenderProgram*          m_RenderProgramPhong;
	RenderProgram*          m_RenderProgramLine;
};
