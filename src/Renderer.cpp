#include "Renderer.h"


Renderer::Renderer():
	m_RenderProgramPhong(nullptr)
{
}

Renderer::~Renderer()
{
	if (m_RenderProgramPhong)
	{
		delete m_RenderProgramPhong;
	}
}


void Renderer::Init()
{
	m_RenderProgramPhong = new RenderProgramPhong;
}

void Renderer::Render(std::vector<RenderObject> &objects, Camera &camera)
{
	for each(auto &object in objects)
	{
		render(const_cast<RenderObject*>(&object), &camera);
	}
}

void Renderer::ClearBuffer()
{
	GetRenderEngine()->ClearVertexBuffer();
}


void Renderer::render(RenderObject* object, Camera* camera)
{
	m_RenderProgramPhong->SetProgramEnable(true);
	bindObject(object);

	m_RenderProgramPhong->SetPNAttrEnable(true);
	setShaderUniform(object, camera);
	if (object->renderBuffer.indicesNum > 0)
	{
		if (object->renderMode == RenderMode::Triangle)
		{
			GetRenderEngine()->DrawElementsTriangle(object->renderBuffer.indicesNum);
		}
		else if (object->renderMode == RenderMode::Line)
		{
			GetRenderEngine()->DrawElementsLine(object->renderBuffer.indicesNum);
		}
	}
	else
	{
		if (object->renderMode == RenderMode::Triangle)
		{
			GetRenderEngine()->DrawArraysTriangle(object->renderBuffer.verticesNum);
		}
		else if (object->renderMode == RenderMode::Line)
		{
			GetRenderEngine()->DrawArraysLine(object->renderBuffer.verticesNum);
		}
	}
	m_RenderProgramPhong->SetPNAttrEnable(false);
	bindObject(nullptr);
	m_RenderProgramPhong->SetProgramEnable(false);
}

void Renderer::bindObject(RenderObject* object)
{
	if (object)
	{
		GetRenderEngine()->BindVerticesBuffer(object->renderBuffer.verticesBuffer);
		if (object->renderBuffer.indicesNum > 0)
		{
			GetRenderEngine()->BindIndicesBuffer(object->renderBuffer.indicesBuffer);
		}
		else
		{
			GetRenderEngine()->BindIndicesBuffer(0);
		}
	}
	else
	{
		GetRenderEngine()->BindVerticesBuffer(0);
		GetRenderEngine()->BindIndicesBuffer(0);
	}
}

void Renderer::setShaderUniform(RenderObject* object, Camera* camera)
{
	m_RenderProgramPhong->SetObjectModelMatrix(object->renderData.modelMatrix);
	m_RenderProgramPhong->SetCameraViewMatrix(camera->GetViewMatrix());
	m_RenderProgramPhong->SetCameraProjectionMatrix(camera->GetProjectionMatrix());

	auto iter = std::find(m_PickedObjIds.begin(), m_PickedObjIds.end(), object->id);
	if (iter != m_PickedObjIds.end())
	{
		m_RenderProgramPhong->SetObjectColor(glm::vec3(0.5f, 0.5f, 0.5f));
	}
	else
	{
		m_RenderProgramPhong->SetObjectColor(object->renderData.color);
	}

	m_RenderProgramPhong->SetCameraPosition(camera->GetEye());
	m_RenderProgramPhong->SetLightPosition(camera->GetEye() + glm::vec3(10.0, 10.0, 10.0));
	m_RenderProgramPhong->SetLightColor(glm::vec3(1.0, 1.0, 1.0));
}
