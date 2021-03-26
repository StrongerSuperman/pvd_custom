#include "core/render/RenderObject.hpp"


RenderData::RenderData(const RenderData &renderData)
{
	color = renderData.color;
	modelMatrix = renderData.modelMatrix;
}

RenderData& RenderData::operator=(const RenderData& renderData)
{
	if (this != &renderData)
	{
		color = renderData.color;
		modelMatrix = renderData.modelMatrix;
	}

	return *this;
}


RenderData::RenderData()
{
}

RenderObject::RenderObject(const RenderObject &renderObject)
{
	id = renderObject.id;
	renderData = renderObject.renderData;
	renderBuffer = renderObject.renderBuffer;
	renderMode = renderObject.renderMode;
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

RenderObject& RenderObject::operator=(const RenderObject& renderObject)
{
	if (this != &renderObject)
	{
		id = renderObject.id;
		renderData = renderObject.renderData;
		renderBuffer = renderObject.renderBuffer;
		renderMode = renderObject.renderMode;
	}

	return *this;
}
