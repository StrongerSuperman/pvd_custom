#pragma once

#include <mutex>
#include <vector>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <QDebug>
#include <QOpenGLFunctions>


enum IndicesType
{
	U32,
	U16,
	U8
};


class RenderEngine : public QOpenGLFunctions
{
public:
	static RenderEngine* GetInstance();

	void UseProgram(GLuint program);
	void BindVerticesBuffer(GLuint verticesBuffer);
	void BindIndicesBuffer(GLuint indicesBuffer);
	void SetVector3f(GLuint uniform, const glm::vec3& vec3);
	void SetMatrix4f(GLuint uniform, const glm::mat4x4& mat);

	void SetPNAttrEnable(GLint position, GLint normal, bool enable);
	void SetPAttrEnable(GLint position, bool enable);

	void DrawElementsTriangle(uint count);
	void DrawArraysTriangle(uint count);
	void DrawElementsLine(uint count);
	void DrawArraysLine(uint count);

	GLuint CreateShader(const char *shaderText, GLenum type);
	GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint CreateVertexBuffer(GLenum type, uint size, const void* data);

	void ClearVertexBuffer();
	GLuint CreatePNVerticesBuffer(uint vertexNum, const void* vertices);
	GLuint CreatePNVerticesBuffer2(uint vertexNum, const void* vertices, uint indexNum, const void* indices, IndicesType indicesType);
	GLuint CreateIndicesBuffer(uint indexNum, const void* indices, IndicesType indicesType);

private:
	RenderEngine();
	RenderEngine(const RenderEngine &) = delete;
	virtual RenderEngine& operator=(const RenderEngine &) = delete;
	~RenderEngine();

	static RenderEngine*       m_Instance;
	static std::mutex          m_Mutex;

	std::vector<GLuint>        m_VertexBuffers;
};

static void GenNormalFromVertex(uint vertexNum, const void* vertices, uint indexNum, const void* indices, IndicesType indicesType, std::vector<glm::vec3>& buffer);