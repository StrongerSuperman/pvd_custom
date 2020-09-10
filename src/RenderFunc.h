#pragma once

#include <mutex>
#include <vector>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <QOpenGLFunctions>


class RenderFunc : public QOpenGLFunctions
{
public:
	static RenderFunc* GetInstance();

	void UseProgram(GLuint program);
	void BindVerticesBuffer(GLuint verticesBuffer);
	void BindIndicesBuffer(GLuint indicesBuffer);
	void SetPositionNormalAttrEnable(GLint position, GLint normal, bool enable);
	void SetVector3f(GLuint uniform, const glm::vec3& vec3);
	void SetMatrix4f(GLuint uniform, const glm::mat4x4& mat);
	void DrawElements(uint count);
	void DrawArrays(uint count);

	GLuint CreateShader(const char *shaderText, GLenum type);
	GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint CreateVerticesBuffer(uint vertexNum, const void* vertices);
	GLuint CreateVerticesBufferWithGenNormal(uint vertexNum, const void* vertices, uint indexNum, const void* indices, bool has16BitIndices);
	GLuint CreateIndicesBuffer(uint indexNum, const void* indices, bool has16BitIndices);
	GLuint CreateVertexBuffer(GLenum type, uint size, const void* data);

private:
	RenderFunc();
	RenderFunc(const RenderFunc &) = delete;
	virtual RenderFunc& operator=(const RenderFunc &) = delete;
	~RenderFunc();

	static RenderFunc*         m_Instance;
	static std::mutex          m_Mutex;

	std::vector<GLuint>        m_VertexBuffers;
};

static void GenNormalFromVertex(uint vertexNum, const void* vertices, uint indexNum, const void* indices, bool has16BitIndices, std::vector<glm::vec3>& buffer);
