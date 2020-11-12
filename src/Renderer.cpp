#include "Renderer.h"


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}


void Renderer::setObjectColor(const RenderObject* object, const ICamera* camera)
{
	(void)camera;
	auto iter = std::find(m_PickedObjIds.begin(), m_PickedObjIds.end(), object->id);
	if (iter != m_PickedObjIds.end())
	{
		auto orgColor = object->renderData.color;
		auto color = glm::vec3(orgColor.x + 0.3f, orgColor.y + 0.3f, orgColor.z + 0.3f);
		m_RenderProgramPhong->SetObjectColor(color);
	}
	else
	{
		m_RenderProgramPhong->SetObjectColor(object->renderData.color);
	}
}
