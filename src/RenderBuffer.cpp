#include "RenderBuffer.h"


RenderBuffer::RenderBuffer(GLuint verticesBuffer, GLuint indicesBuffer, GLuint verticesNum, GLuint indicesNum):
	verticesBuffer(verticesBuffer),
	indicesBuffer(indicesBuffer),
	verticesNum(verticesNum),
	indicesNum(indicesNum)
{
}

RenderBuffer::~RenderBuffer()
{
}


RenderBuffer CreateRenderBuffer(const void* vertices, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices)
{
	auto verticesBuffer = RenderEngine::GetInstance()->CreatePNVerticesBuffer(vertexNum, vertices);
	return DoCreateRenderBuffer(verticesBuffer, vertexNum, indices, indexNum, has16BitIndices);
}

RenderBuffer CreateRenderBuffer2(const void* vertices, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices)
{
	auto verticesBuffer = RenderEngine::GetInstance()->CreatePNVerticesBuffer2(vertexNum, vertices, indexNum, indices, has16BitIndices);
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
