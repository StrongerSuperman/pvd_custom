#include "RenderObject.h"


RenderObject CreateRenderObject(uint id, const void* vertices, uint vertexNum, const void* indices, uint indexNum,
	const glm::mat4x4& modelMatrix, const glm::vec3& color, bool has16BitIndices)
{
	return RenderObject
	{
		id,
		modelMatrix,
		color,
		CreateRenderBuffer(vertices, vertexNum, indices, indexNum, has16BitIndices),
	};
}

RenderObject CreateRenderObjectWithGenNormal(uint id, const void* vertices, uint vertexNum, const void* indices, uint indexNum,
	const glm::mat4x4& modelMatrix, const glm::vec3& color, bool has16BitIndices)
{
	return RenderObject
	{
		id,
		modelMatrix,
		color,
		CreateRenderBufferWithGenNormal(vertices, vertexNum, indices, indexNum, has16BitIndices),
	};
}

RenderBuffer CreateRenderBuffer(const void* vertices, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices)
{
	GLuint verticesBuffer = RenderEngine::GetInstance()->CreateVerticesBuffer(vertexNum, vertices);
	return DoCreateRenderBuffer(verticesBuffer, vertexNum, indices, indexNum, has16BitIndices);
}

RenderBuffer CreateRenderBufferWithGenNormal(const void* vertices, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices)
{
	GLuint verticesBuffer = RenderEngine::GetInstance()->CreateVerticesBufferWithGenNormal(vertexNum, vertices, indexNum, indices, has16BitIndices);
	return DoCreateRenderBuffer(verticesBuffer, vertexNum, indices, indexNum, has16BitIndices);
}

RenderBuffer DoCreateRenderBuffer(GLuint verticesBuffer, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices)
{
	GLuint indicesBuffer;
	if (indexNum > 0)
	{
		indicesBuffer = RenderEngine::GetInstance()->CreateIndicesBuffer(indexNum, indices, has16BitIndices);
	}
	else
	{
		indicesBuffer = -1;
	}

	return RenderBuffer
	{
		verticesBuffer,
		indicesBuffer,
		vertexNum,
		indexNum,
	};
}
