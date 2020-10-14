#include "RenderBuffer.h"


RenderBuffer::RenderBuffer()
{
}

RenderBuffer::RenderBuffer(GLuint verticesBuffer, GLuint indicesBuffer, GLuint verticesNum, GLuint indicesNum):
	verticesBuffer(verticesBuffer),
	indicesBuffer(indicesBuffer),
	verticesNum(verticesNum),
	indicesNum(indicesNum)
{
}

RenderBuffer::RenderBuffer(const RenderBuffer& renderBuffer)
{
	verticesBuffer = renderBuffer.verticesBuffer;
	indicesBuffer = renderBuffer.indicesBuffer;
	verticesNum = renderBuffer.verticesNum;
	indicesNum = renderBuffer.indicesNum;
}

RenderBuffer& RenderBuffer::operator=(const RenderBuffer& renderBuffer)
{
	if (this != &renderBuffer)
	{
		verticesBuffer = renderBuffer.verticesBuffer;
		indicesBuffer = renderBuffer.indicesBuffer;
		verticesNum = renderBuffer.verticesNum;
		indicesNum = renderBuffer.indicesNum;
	}

	return *this;
}


RenderBuffer CreateRenderBuffer(const void* vertices, uint vertexNum, const void* indices, uint indexNum, IndicesType indicesType)
{
	auto verticesBuffer = RenderEngine::GetInstance()->CreatePNVerticesBuffer(vertexNum, vertices);
	return DoCreateRenderBuffer(verticesBuffer, vertexNum, indices, indexNum, indicesType);
}

RenderBuffer CreateRenderBuffer2(const void* vertices, uint vertexNum, const void* indices, uint indexNum, IndicesType indicesType)
{
	auto verticesBuffer = RenderEngine::GetInstance()->CreatePNVerticesBuffer2(vertexNum, vertices, indexNum, indices, indicesType);
	return DoCreateRenderBuffer(verticesBuffer, vertexNum, indices, indexNum, indicesType);
}

RenderBuffer DoCreateRenderBuffer(GLuint verticesBuffer, uint vertexNum, const void* indices, uint indexNum, IndicesType indicesType)
{
	GLuint indicesBuffer;
	if (indexNum > 0)
	{
		indicesBuffer = RenderEngine::GetInstance()->CreateIndicesBuffer(indexNum, indices, indicesType);
	}
	else
	{
		indicesBuffer = -1;
	}

	return RenderBuffer(verticesBuffer, indicesBuffer, vertexNum, indexNum);
}
