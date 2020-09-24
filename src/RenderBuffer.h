#pragma once

#include "RenderEngine.h"


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


RenderBuffer CreateRenderBuffer(const void* vertices, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices = false);

RenderBuffer CreateRenderBuffer2(const void* vertices, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices = false);

RenderBuffer DoCreateRenderBuffer(GLuint verticesBuffer, uint vertexNum, const void* indices, uint indexNum, bool has16BitIndices);
