#include "Renderer.h"


Renderer::Renderer():
	m_RenderProgramPhong(nullptr),
	m_RenderProgramLine(nullptr)
{
}

Renderer::~Renderer()
{
	delete m_RenderProgramPhong;
	delete m_RenderProgramLine;
}

void Renderer::Init()
{
	m_RenderProgramPhong = new RenderProgramPhong;
	m_RenderProgramLine = new RenderProgramLine;
}

void Renderer::Render(std::vector<RenderObject> &objects, Camera &camera)
{
	for each(auto &object in objects)
	{
		m_RenderProgramPhong->Render(object, camera);
	}
}

void Renderer::RenderLine(std::vector<RenderObject> &objects, Camera &camera)
{
	for each(auto &object in objects)
	{
		m_RenderProgramLine->Render(object, camera);
	}
}

void Renderer::SetPickedRenderObjectIds(const std::vector<int>& ids)
{
	m_RenderProgramPhong->SetPickedRenderObjects(ids);
}
