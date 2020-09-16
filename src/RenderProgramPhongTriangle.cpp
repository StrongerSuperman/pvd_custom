#include "RenderProgramPhongTriangle.h"


RenderProgramPhongTriangle::RenderProgramPhongTriangle()
{
}

RenderProgramPhongTriangle::~RenderProgramPhongTriangle()
{
}

void RenderProgramPhongTriangle::Render(const RenderObject& object, const Camera& camera)
{
	StartProgram(object);

	SetVetexAttrEnable(true);
	SetShaderUniform(object, camera);
	if (object.RenderBuffer.IndicesNum > 0)
	{
		GetRenderEngine()->DrawElementsTriangle(object.RenderBuffer.IndicesNum);
	}
	else
	{
		GetRenderEngine()->DrawArraysTriangle(object.RenderBuffer.VerticesNum);
	}
	SetVetexAttrEnable(false);

	EndProgram();
}


void RenderProgramPhongTriangle::SetVetexAttrEnable(bool enable)
{
	GetRenderEngine()->SetPNAttrTriangleEnable(GetAttributePosition(), GetAttributeNormal(), enable);
}
