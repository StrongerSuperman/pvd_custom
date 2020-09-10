#pragma once

#include "RenderFunc.h"


class RenderBuffer
{
public:
	GLuint      VerticesBuffer;
	GLuint      IndicesBuffer;
	uint        VerticesNum;
	uint        IndicesNum;
};


class RenderObject
{
public:
	uint             Id;
	glm::mat4x4      ModelMatrix;
	glm::vec3        Color;
	RenderBuffer     RenderBuffer;
};


RenderObject CreateRenderObject(uint id, const void* vertices, uint vertexNum, const void* indices, uint indexNum,
	const glm::mat4x4& modelMatrix, const glm::vec3& color, bool has16BitIndices = false);

RenderObject CreateRenderObjectWithGenNormal(uint id, const void* vertices, uint vertexNum, const void* indices, uint indexNum,
	const glm::mat4x4& modelMatrix, const glm::vec3& color, bool has16BitIndices = false);

RenderBuffer CreateRenderBuffer(const void* vertices, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices = false);

RenderBuffer CreateRenderBufferWithGenNormal(const void* vertices, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices = false);

RenderBuffer DoCreateRenderBuffer(GLuint verticesBuffer, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices);
