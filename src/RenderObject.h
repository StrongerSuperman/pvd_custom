#pragma once

#include "RenderBuffer.h"
#include "RenderEngine.h"


enum RenderMode
{
	Triangle,
	Line,
};

class RenderData
{
public:
	RenderData();
	RenderData(const RenderData &renderData);
	RenderData(const glm::vec3& color, const glm::mat4x4& modelMatrix);
	RenderData& operator=(const RenderData& renderData);

	mutable glm::vec3    color;
	mutable glm::mat4x4  modelMatrix;
};

class RenderObject
{
public:
	RenderObject();
	RenderObject(const RenderObject &renderObject);
	RenderObject(uint id, const RenderData& renderData, const RenderBuffer& renderBuffer, RenderMode renderMode = RenderMode::Triangle);
	RenderObject& operator=(const RenderObject& renderObject);

	mutable uint        id;
	mutable RenderData  renderData;
	RenderBuffer        renderBuffer;
	RenderMode          renderMode;
};
