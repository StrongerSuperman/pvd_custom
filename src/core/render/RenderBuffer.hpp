#pragma once

#include "core/render/RenderEngine.hpp"


struct RenderBuffer
{
public:
	RenderBuffer();
	RenderBuffer(const RenderBuffer& renderBuffer);
	RenderBuffer(GLuint verticesBuffer, GLuint indicesBuffer, GLuint verticesNum, GLuint indicesNum);
	RenderBuffer& operator=(const RenderBuffer& renderBuffer);

	GLuint      verticesBuffer;
	GLuint      indicesBuffer;
	uint        verticesNum;
	uint        indicesNum;
};


RenderBuffer CreateRenderBuffer(const void* vertices, uint vertexNum, const void* indices, uint indexNum, IndicesType indicesType = U32);

RenderBuffer CreateRenderBuffer2(const void* vertices, uint vertexNum, const void* indices, uint indexNum, IndicesType indicesType = U32);

RenderBuffer DoCreateRenderBuffer(GLuint verticesBuffer, uint vertexNum, const void* indices, uint indexNum, IndicesType indicesType);
