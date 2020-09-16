#include "Renderer.h"


Renderer::Renderer():
	m_RenderProgramPhongTriangle(nullptr),
	m_RenderProgramPhongLine(nullptr)
{
}

Renderer::~Renderer()
{
	delete m_RenderProgramPhongTriangle;
	delete m_RenderProgramPhongLine;
}

void Renderer::Init()
{
	m_RenderProgramPhongTriangle = new RenderProgramPhongTriangle;
	m_RenderProgramPhongLine = new RenderProgramPhongLine;
}

void Renderer::Render(std::vector<RenderObject> &objects, Camera &camera)
{
	for each(auto &object in objects)
	{
		m_RenderProgramPhongTriangle->Render(object, camera);
	}
}

void Renderer::RenderLine(std::vector<RenderObject> &objects, Camera &camera)
{
	for each(auto &object in objects)
	{
		m_RenderProgramPhongLine->Render(object, camera);
	}
}

void Renderer::SetPickedRenderObjectIds(const std::vector<int>& ids)
{
	m_RenderProgramPhongTriangle->SetPickedRenderObjects(ids);
}
