#include "RenderProgramPhongLine.h"


RenderProgramPhongLine::RenderProgramPhongLine()
{
}

RenderProgramPhongLine::~RenderProgramPhongLine()
{
}

void RenderProgramPhongLine::Render(const RenderObject& object, const Camera& camera)
{
	StartProgram(object);

	SetVetexAttrEnable(true);
	SetShaderUniform(object, camera);
	if (object.RenderBuffer.IndicesNum > 0)
	{
		GetRenderEngine()->DrawElementsLine(object.RenderBuffer.IndicesNum / 2);
	}
	else
	{
		GetRenderEngine()->DrawArraysLine(object.RenderBuffer.VerticesNum / 2);
	}
	SetVetexAttrEnable(false);

	EndProgram();
}


void RenderProgramPhongLine::SetVetexAttrEnable(bool enable)
{
	GetRenderEngine()->SetPNAttrTriangleEnable(GetAttributePosition(), GetAttributeNormal(), enable);
}

