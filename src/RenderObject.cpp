#include "RenderObject.h"


RenderData::RenderData()
{
}

RenderData::RenderData(const glm::vec3& color, const glm::mat4x4& modelMatrix) :
	color(color),
	modelMatrix(modelMatrix)
{
}

RenderObject::RenderObject():
	renderMode(RenderMode::Triangle),
	renderBuffer(RenderBuffer(-1, -1, 0, 0))
{
}

RenderObject::RenderObject(uint id, const RenderData& renderData, const RenderBuffer& renderBuffer, RenderMode renderMode) :
	id(id),
	renderData(renderData),
	renderBuffer(renderBuffer),
	renderMode(renderMode)
{
}

RenderObject::~RenderObject()
{
}
