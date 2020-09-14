#include "RenderFunc.h"


RenderFunc* RenderFunc::GetInstance()
{
	static RenderFunc*  m_Instance;
	static std::mutex   m_Mutex;

	if (!m_Instance)
	{
		m_Mutex.lock();
		if (!m_Instance)
		{
			m_Instance = new RenderFunc;
		}
		m_Mutex.unlock();
	}

	return m_Instance;
}


void RenderFunc::UseProgram(GLuint program)
{
	glUseProgram(program);
}

void RenderFunc::BindVerticesBuffer(GLuint verticesBuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
}

void RenderFunc::BindIndicesBuffer(GLuint indicesBuffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
}

void RenderFunc::SetPositionNormalAttrEnable(GLint position, GLint normal, bool enable)
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

void RenderFunc::SetPositionAttrEnable(GLint position, bool enable)
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

void RenderFunc::SetVector3f(GLuint uniform, const glm::vec3& vec3)
{
	glUniform3f(uniform, vec3.x, vec3.y, vec3.z);
}

void RenderFunc::SetMatrix4f(GLuint uniform, const glm::mat4x4& mat)
{
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
}

void RenderFunc::DrawElementsTriangle(uint count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL);
}

void RenderFunc::DrawArraysTriangle(uint count)
{
	glDrawArrays(GL_TRIANGLES, 0, count);
}

void RenderFunc::DrawElementsLine(uint count)
{
	glDrawElements(GL_LINES, count, GL_UNSIGNED_SHORT, NULL);
}

void RenderFunc::DrawArraysLine(uint count)
{
	glDrawArrays(GL_LINES, 0, count);
}


GLuint RenderFunc::CreateShader(const char *shaderText, GLenum type)
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
			std::cout << log << "\n";
		}
	}

	return shader;
}

GLuint RenderFunc::CreateProgram(GLuint vertexShader, GLuint fragmentShader)
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
			std::cout << log << "\n";
		}
	}

	return program;
}

GLuint RenderFunc::CreateVertexBuffer(GLenum type, uint size, const void* data)
{
	GLuint vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(type, vertexBuffer);
	glBufferData(type, size, data, GL_STATIC_DRAW);
	glBindBuffer(type, 0);

	m_VertexBuffers.push_back(vertexBuffer);

	return vertexBuffer;
}

GLuint RenderFunc::CreateVerticesBufferWithGenNormal(uint vertexNum, const void* vertices, uint indexNum, const void* indices, bool has16BitIndices)
{
	std::vector<glm::vec3> buffer(vertexNum * 2);
	GenNormalFromVertex(vertexNum, vertices, indexNum, indices, has16BitIndices, buffer);

	const void* data = reinterpret_cast<const void*>(&buffer[0]);
	return CreateVertexBuffer(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertexNum, data);
}

GLuint RenderFunc::CreateVerticesBuffer(uint vertexNum, const void* vertices)
{
	return CreateVertexBuffer(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertexNum, vertices);
}

GLuint RenderFunc::CreateIndicesBuffer(uint indexNum, const void* indices, bool has16BitIndices)
{
	if (has16BitIndices)
	{
		return CreateVertexBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * indexNum, indices);
	}
	else
	{
		return CreateVertexBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indexNum, indices);
	}
}


RenderFunc::RenderFunc()
{
	initializeOpenGLFunctions();
}

RenderFunc::~RenderFunc()
{
	for each (auto vertexBuffer in m_VertexBuffers)
	{
		glDeleteBuffers(1, &vertexBuffer);
	}
}


void GenNormalFromVertex(uint vertexNum, const void* vertices, uint indexNum, const void* indices, bool has16BitIndices, std::vector<glm::vec3>& buffer)
{
	(void)(vertexNum);
	const glm::vec3* verticesCast = reinterpret_cast<const glm::vec3*>(vertices);
	const short* shortIndices = reinterpret_cast<const short*>(indices);
	const int* intIndices = reinterpret_cast<const int*>(indices);

	for (uint i = 0; i < indexNum / 3; ++i)
	{
		glm::vec3 tVertex[3];
		if (has16BitIndices)
		{
			tVertex[0] = verticesCast[*shortIndices++];
			tVertex[1] = verticesCast[*shortIndices++];
			tVertex[2] = verticesCast[*shortIndices++];
		}
		else
		{
			tVertex[0] = verticesCast[*intIndices++];
			tVertex[1] = verticesCast[*intIndices++];
			tVertex[2] = verticesCast[*intIndices++];
		}
		glm::vec3 fNormal = glm::normalize(glm::cross(tVertex[1] - tVertex[0], tVertex[2] - tVertex[0]));

		if (has16BitIndices)
		{
			int idx1 = *(shortIndices - 1);
			int idx2 = *(shortIndices - 2);
			int idx3 = *(shortIndices - 3);
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
				buffer[idx1 * 2 + 1] = (buffer[idx1 * 2 + 1] + fNormal).getNormalized();
				buffer[idx2 * 2] = tVertex[1];
				buffer[idx2 * 2 + 1] = (buffer[idx2 * 2 + 1] + fNormal).getNormalized();
				buffer[idx3 * 2] = tVertex[0];
				buffer[idx3 * 2 + 1] = (buffer[idx3 * 2 + 1] + fNormal).getNormalized();
			}
#endif // AVERAGE_NORM
		}
		else
		{
			int idx1 = *(intIndices - 1);
			int idx2 = *(intIndices - 2);
			int idx3 = *(intIndices - 3);
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
				buffer[idx1 * 2 + 1] = (buffer[idx1 * 2 + 1] + fNormal).getNormalized();
				buffer[idx2 * 2] = tVertex[1];
				buffer[idx2 * 2 + 1] = (buffer[idx2 * 2 + 1] + fNormal).getNormalized();
				buffer[idx3 * 2] = tVertex[0];
				buffer[idx3 * 2 + 1] = (buffer[idx3 * 2 + 1] + fNormal).getNormalized();
			}
#endif // AVERAGE_NORM
		}
	}
}
