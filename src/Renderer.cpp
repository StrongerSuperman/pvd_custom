#include "Renderer.h"


Renderer::Renderer():
	m_RenderProgram(nullptr)
{
}

Renderer::~Renderer()
{
	delete m_RenderProgram;
}

void Renderer::Init()
{
	m_RenderProgram = new RenderProgramPhong;
	m_RenderProgram->Init();
}

void Renderer::Render(std::vector<RenderObject> &objects, Camera &camera)
{
	for each(auto &object in objects)
	{
		m_RenderProgram->Render(object, camera);
	}
}

void Renderer::SetPickedRenderObjectId(int id)
{
	m_RenderProgram->SetPickedRenderObject(id);
}
