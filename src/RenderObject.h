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
	explicit RenderData();
	explicit RenderData(const glm::vec3& color, const glm::mat4x4& modelMatrix);

	mutable glm::vec3        color;
	mutable glm::mat4x4      modelMatrix;
};

class RenderObject
{
public:
	explicit RenderObject();
	explicit RenderObject(uint id, const RenderData& renderData, const RenderBuffer& renderBuffer, RenderMode renderMode = RenderMode::Triangle);
	~RenderObject();

	mutable uint             id;
	mutable RenderData       renderData;
	RenderBuffer             renderBuffer;
	RenderMode               renderMode;
};
