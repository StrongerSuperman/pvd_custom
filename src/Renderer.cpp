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
		auto iter = std::find(m_PickedObjIds.begin(), m_PickedObjIds.end(), object.Id);
		if (iter != m_PickedObjIds.end())
		{
			object.Color = glm::vec3(0.5f, 0.5f, 0.5f);
		}
		render(const_cast<RenderObject*>(&object), &camera);
	}
}

void Renderer::RenderLine(std::vector<RenderObject> &objects, Camera &camera)
{
	for each(auto &object in objects)
	{
		renderLine(const_cast<RenderObject*>(&object), &camera);
	}
}


void Renderer::render(RenderObject* object, Camera* camera)
{
	m_RenderProgramPhong->SetProgramEnable(true);
	bindObject(object);

	m_RenderProgramPhong->SetPNAttrEnable(true);
	setShaderUniform(object, camera);
	if (object->RenderBuffer.IndicesNum > 0)
	{
		GetRenderEngine()->DrawElementsTriangle(object->RenderBuffer.IndicesNum);
	}
	else
	{
		GetRenderEngine()->DrawArraysTriangle(object->RenderBuffer.VerticesNum);
	}
	m_RenderProgramPhong->SetPNAttrEnable(false);
	bindObject(nullptr);
	m_RenderProgramPhong->SetProgramEnable(false);
}

void Renderer::renderLine(RenderObject* object, Camera* camera)
{
	m_RenderProgramPhong->SetProgramEnable(true);
	bindObject(object);
	m_RenderProgramPhong->SetPNAttrEnable(true);
	setShaderUniform(object, camera);
	if (object->RenderBuffer.IndicesNum > 0)
	{
		GetRenderEngine()->DrawElementsLine(object->RenderBuffer.IndicesNum);
	}
	else
	{
		GetRenderEngine()->DrawArraysLine(object->RenderBuffer.VerticesNum);
	}
	m_RenderProgramPhong->SetPNAttrEnable(false);
	bindObject(nullptr);
	m_RenderProgramPhong->SetProgramEnable(false);
}


void Renderer::bindObject(RenderObject* object)
{
	if (object)
	{
		GetRenderEngine()->BindVerticesBuffer(object->RenderBuffer.VerticesBuffer);
		if (object->RenderBuffer.IndicesNum > 0)
		{
			GetRenderEngine()->BindIndicesBuffer(object->RenderBuffer.IndicesBuffer);
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
	m_RenderProgramPhong->SetObjectModelMatrix(object->ModelMatrix);
	m_RenderProgramPhong->SetCameraViewMatrix(camera->GetViewMatrix());
	m_RenderProgramPhong->SetCameraProjectionMatrix(camera->GetProjectionMatrix());
	m_RenderProgramPhong->SetObjectColor(object->Color);
	m_RenderProgramPhong->SetCameraPosition(camera->GetEye());
	m_RenderProgramPhong->SetLightPosition(camera->GetEye() + glm::vec3(10.0, 10.0, 10.0));
	m_RenderProgramPhong->SetLightColor(glm::vec3(1.0, 1.0, 1.0));
}
