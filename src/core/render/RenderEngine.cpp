#include "core/render/RenderEngine.hpp"


RenderEngine* RenderEngine::GetInstance()
{
	static RenderEngine*  m_Instance;
	static std::mutex     m_Mutex;

	if (!m_Instance)
	{
		m_Mutex.lock();
		if (!m_Instance)
		{
			m_Instance = new RenderEngine;
		}
		m_Mutex.unlock();
	}

	return m_Instance;
}


void RenderEngine::UseProgram(GLuint program)
{
	glUseProgram(program);
}

void RenderEngine::BindVerticesBuffer(GLuint verticesBuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
}

void RenderEngine::BindIndicesBuffer(GLuint indicesBuffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
}

void RenderEngine::SetVector3f(GLuint uniform, const glm::vec3& vec3)
{
	glUniform3f(uniform, vec3.x, vec3.y, vec3.z);
}

void RenderEngine::SetMatrix4f(GLuint uniform, const glm::mat4x4& mat)
{
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
}


void RenderEngine::SetPNAttrEnable(GLint position, GLint normal, bool enable)
{
	if (enable)
	{
		glEnableVertexAttribArray(position);
		glEnableVertexAttribArray(normal);

		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(0 * sizeof(float)));
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	}
	else
	{
		glDisableVertexAttribArray(position);
		glDisableVertexAttribArray(normal);
	}
}

void RenderEngine::SetPAttrEnable(GLint position, bool enable)
{
	if (enable)
	{
		glEnableVertexAttribArray(position);

		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(0 * sizeof(float)));
	}
	else
	{
		glDisableVertexAttribArray(position);
	}
}


void RenderEngine::DrawElementsTriangle(uint count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL);
}

void RenderEngine::DrawArraysTriangle(uint count)
{
	glDrawArrays(GL_TRIANGLES, 0, count);
}

void RenderEngine::DrawElementsLine(uint count)
{
	glDrawElements(GL_LINES, count, GL_UNSIGNED_SHORT, NULL);
}

void RenderEngine::DrawArraysLine(uint count)
{
	glDrawArrays(GL_LINES, 0, count);
}


GLuint RenderEngine::CreateShader(const char *shaderText, GLenum type)
{
	GLint status = 0;
	GLint infoLength = 0;
	char* log;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderText, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

	if (infoLength > 0)
	{
		log = new char[infoLength + 0];
		glGetShaderInfoLog(shader, infoLength, &infoLength, log);
		log[infoLength] = 0;

		if (log[0] != 0)
		{
			//std::cout << log << "\n";
			qDebug() << log << "\n";
		}
	}

	return shader;
}

GLuint RenderEngine::CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLint status = 0;
	GLint infoLength = 0;
	char* log;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);

	if (infoLength > 0)
	{
		log = new char[infoLength + 0];
		glGetProgramInfoLog(program, infoLength, &infoLength, log);
		log[infoLength] = 0;

		if (log[0] != 0)
		{
			//std::cout << log << "\n";
			qDebug() << log << "\n";
		}
	}

	return program;
}

GLuint RenderEngine::CreateVertexBuffer(GLenum type, uint size, const void* data)
{
	GLuint vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(type, vertexBuffer);
	glBufferData(type, size, data, GL_STATIC_DRAW);
	glBindBuffer(type, 0);

	m_VertexBuffers.push_back(vertexBuffer);

	return vertexBuffer;
}


void RenderEngine::ClearVertexBuffer()
{
	for each (auto vertexBuffer in m_VertexBuffers)
	{
		glDeleteBuffers(1, &vertexBuffer);
	}
	m_VertexBuffers.clear();
}

GLuint RenderEngine::CreatePNVerticesBuffer(uint vertexNum, const void* vertices)
{
	return CreateVertexBuffer(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertexNum, vertices);
}

GLuint RenderEngine::CreatePNVerticesBuffer2(uint vertexNum, const void* vertices, uint indexNum, const void* indices, IndicesType indicesType)
{
	std::vector<glm::vec3> buffer(vertexNum * 2);
	GenNormalFromVertex(vertexNum, vertices, indexNum, indices, indicesType, buffer);

	const void* data = reinterpret_cast<const void*>(&buffer[0]);
	return CreateVertexBuffer(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertexNum, data);
}

GLuint RenderEngine::CreateIndicesBuffer(uint indexNum, const void* indices, IndicesType indicesType)
{
	if (indicesType == U8)
	{
		return CreateVertexBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(char) * indexNum, indices);
	}
	else if (indicesType == U16)
	{
		return CreateVertexBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * indexNum, indices);
	}
	else if(indicesType == U32)
	{
		return CreateVertexBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indexNum, indices);
	}

	assert(0), "Undefined indices type!";
	return -1;
}


RenderEngine::RenderEngine()
{
	initializeOpenGLFunctions();
}

RenderEngine::~RenderEngine()
{
	ClearVertexBuffer();
}


void GenNormalFromVertex(uint vertexNum, const void* vertices, uint indexNum, const void* indices, IndicesType indicesType, std::vector<glm::vec3>& buffer)
{
	(void)(vertexNum);
	const glm::vec3* verticesCast = reinterpret_cast<const glm::vec3*>(vertices);
	const char* u8Indices = reinterpret_cast<const char*>(indices);
	const short* u16Indices = reinterpret_cast<const short*>(indices);
	const int* u32Indices = reinterpret_cast<const int*>(indices);

	for (uint i = 0; i < indexNum / 3; ++i)
	{
		glm::vec3 tVertex[3];
		int idx1, idx2, idx3;
		if (indicesType == U8)
		{
			tVertex[0] = verticesCast[*u8Indices++];
			tVertex[1] = verticesCast[*u8Indices++];
			tVertex[2] = verticesCast[*u8Indices++];
			idx1 = *(u8Indices - 1);
			idx2 = *(u8Indices - 2);
			idx3 = *(u8Indices - 3);
		}
		else if(indicesType == U16)
		{
			tVertex[0] = verticesCast[*u16Indices++];
			tVertex[1] = verticesCast[*u16Indices++];
			tVertex[2] = verticesCast[*u16Indices++];
			idx1 = *(u16Indices - 1);
			idx2 = *(u16Indices - 2);
			idx3 = *(u16Indices - 3);
		}
		else if(indicesType == U32)
		{
			tVertex[0] = verticesCast[*u32Indices++];
			tVertex[1] = verticesCast[*u32Indices++];
			tVertex[2] = verticesCast[*u32Indices++];
			idx1 = *(u32Indices - 1);
			idx2 = *(u32Indices - 2);
			idx3 = *(u32Indices - 3);
		}

		glm::vec3 fNormal = glm::normalize(glm::cross(tVertex[1] - tVertex[0], tVertex[2] - tVertex[0]));

#ifndef AVERAGE_NORM
		buffer[idx1 * 2] = tVertex[2];
		buffer[idx1 * 2 + 1] = fNormal;
		buffer[idx2 * 2] = tVertex[1];
		buffer[idx2 * 2 + 1] = fNormal;
		buffer[idx3 * 2] = tVertex[0];
		buffer[idx3 * 2 + 1] = fNormal;
#else
		if (i == 0)
		{
			buffer[idx1 * 2] = tVertex[2];
			buffer[idx1 * 2 + 1] = fNormal;
			buffer[idx2 * 2] = tVertex[1];
			buffer[idx2 * 2 + 1] = fNormal;
			buffer[idx3 * 2] = tVertex[0];
			buffer[idx3 * 2 + 1] = fNormal;
		}
		else
		{
			buffer[idx1 * 2] = tVertex[2];
			buffer[idx1 * 2 + 1] = glm::normalize(buffer[idx1 * 2 + 1] + fNormal);
			buffer[idx2 * 2] = tVertex[1];
			buffer[idx2 * 2 + 1] = glm::normalize(buffer[idx2 * 2 + 1] + fNormal);
			buffer[idx3 * 2] = tVertex[0];
			buffer[idx3 * 2 + 1] = glm::normalize(buffer[idx3 * 2 + 1] + fNormal);
		}
#endif // AVERAGE_NORM
	}
}
