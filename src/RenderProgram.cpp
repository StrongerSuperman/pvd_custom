#include "RenderProgram.h"


RenderProgram::RenderProgram():
	m_VertexShader(0),
	m_FragmentShader(0),
	m_ShaderProgram(0)
{
}

RenderProgram::~RenderProgram()
{
	GetRenderFunc()->glDeleteProgram(m_ShaderProgram);
	GetRenderFunc()->glDeleteShader(m_VertexShader);
	GetRenderFunc()->glDeleteShader(m_FragmentShader);
}


void RenderProgram::SetProgramEnable(bool enable)
{
	if (enable)
	{
		GetRenderFunc()->UseProgram(GetShaderProgram());
	}
	else
	{
		GetRenderFunc()->UseProgram(0);
	}
}

void RenderProgram::InitShader(const char* vertexShader, const char* fragShader)
{
	m_VertexShader = GetRenderFunc()->CreateShader(vertexShader, GL_VERTEX_SHADER);
	m_FragmentShader = GetRenderFunc()->CreateShader(fragShader, GL_FRAGMENT_SHADER);
	m_ShaderProgram = GetRenderFunc()->CreateProgram(m_VertexShader, m_FragmentShader);

	OnInitShader();
}

void RenderProgram::OnInitShader()
{
}

RenderFunc* RenderProgram::GetRenderFunc()
{
	return RenderFunc::GetInstance();
}

GLuint RenderProgram::GetShaderProgram()
{
	return m_ShaderProgram;
}
