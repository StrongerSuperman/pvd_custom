#include "interface/IRenderer.hpp"


IRenderer::IRenderer() :
	m_RenderProgramPhong(nullptr)
{
}

IRenderer::~IRenderer()
{
	if (m_RenderProgramPhong)
	{
		delete m_RenderProgramPhong;
	}
}


void IRenderer::Init()
{
	m_RenderProgramPhong = new RenderProgramPhong;
}

void IRenderer::Clear()
{
	GetRenderEngine()->ClearVertexBuffer();
}

void IRenderer::Render(const RenderObject &object, const ICamera &camera)
{
	render(&object, &camera);
}


void IRenderer::setObjectColor(const RenderObject* object, const ICamera* camera)
{
	(void)camera;
	m_RenderProgramPhong->SetObjectColor(object->renderData.color);
}

void IRenderer::setLightPosition(const RenderObject* object, const ICamera* camera)
{
	(void)object;
	auto position = camera->GetEye() + glm::vec3(10.0, 10.0, 10.0);
	m_RenderProgramPhong->SetLightPosition(position);
}

void IRenderer::setLightColor(const RenderObject* object, const ICamera* camera)
{
	(void)object;
	(void)camera;
	auto color = glm::vec3(1.0, 1.0, 1.0);
	m_RenderProgramPhong->SetLightColor(color);
}


void IRenderer::render(const RenderObject* object, const ICamera* camera)
{
	bindObject(object);
	sendData(object, camera);
	drawObject(object);
	bindObject(nullptr);
}

void IRenderer::bindObject(const RenderObject* object)
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

void IRenderer::drawObject(const RenderObject* object)
{
	if (object->renderMode == RenderMode::Triangle)
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

void IRenderer::sendData(const RenderObject* object, const ICamera* camera)
{
	m_RenderProgramPhong->SetObjectModelMatrix(object->renderData.modelMatrix);
	m_RenderProgramPhong->SetCameraViewMatrix(camera->GetViewMatrix());
	m_RenderProgramPhong->SetCameraProjectionMatrix(camera->GetProjectionMatrix());
	m_RenderProgramPhong->SetCameraPosition(camera->GetEye());

	setObjectColor(object, camera);
	setLightPosition(object, camera);
	setLightColor(object, camera);
}
