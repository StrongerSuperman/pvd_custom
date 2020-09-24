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

void Renderer::Clear()
{
	GetRenderEngine()->ClearVertexBuffer();
}

void Renderer::Render(const RenderObject &object, const Camera &camera)
{
	render(&object, &camera);
}


void Renderer::render(const RenderObject* object, const Camera* camera)
{
	bindObject(object);
	sendData(object, camera);
	drawObject(object);
	bindObject(nullptr);
}

void Renderer::bindObject(const RenderObject* object)
{
	if (object)
	{
		m_RenderProgramPhong->SetProgramEnable(true);

		GetRenderEngine()->BindVerticesBuffer(object->renderBuffer.verticesBuffer);
		if (object->renderBuffer.indicesNum > 0)
		{
			GetRenderEngine()->BindIndicesBuffer(object->renderBuffer.indicesBuffer);
		}
		else
		{
			GetRenderEngine()->BindIndicesBuffer(0);
		}

		m_RenderProgramPhong->SetPNAttrEnable(true);
	}
	else
	{
		m_RenderProgramPhong->SetPNAttrEnable(false);

		GetRenderEngine()->BindVerticesBuffer(0);
		GetRenderEngine()->BindIndicesBuffer(0);

		m_RenderProgramPhong->SetProgramEnable(false);
	}
}

void Renderer::drawObject(const RenderObject* object)
{
	if(object->renderMode == RenderMode::Triangle)
	{
		if (object->renderBuffer.indicesNum > 0)
		{
			GetRenderEngine()->DrawElementsTriangle(object->renderBuffer.indicesNum);
		}
		else
		{
			GetRenderEngine()->DrawArraysTriangle(object->renderBuffer.verticesNum);
		}
	}
	else if (object->renderMode == RenderMode::Line)
	{
		if (object->renderBuffer.indicesNum > 0)
		{
			GetRenderEngine()->DrawElementsLine(object->renderBuffer.indicesNum);
		}
		else
		{
			GetRenderEngine()->DrawArraysLine(object->renderBuffer.verticesNum);
		}
	}
}

void Renderer::sendData(const RenderObject* object, const Camera* camera)
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
