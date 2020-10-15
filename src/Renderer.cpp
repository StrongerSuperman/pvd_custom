#include "Renderer.h"


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}


void Renderer::setObjectColor(const RenderObject* object)
{
	auto iter = std::find(m_PickedObjIds.begin(), m_PickedObjIds.end(), object->id);
	if (iter != m_PickedObjIds.end())
	{
		m_RenderProgramPhong->SetObjectColor(glm::vec3(0.5f, 0.5f, 0.5f));
	}
	else
	{
		m_RenderProgramPhong->SetObjectColor(object->renderData.color);
	}
}

