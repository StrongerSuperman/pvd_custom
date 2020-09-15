#include "RenderProgram.h"


RenderProgram::RenderProgram():
	m_VertexShader(0),
	m_FragmentShader(0),
	m_ShaderProgram(0)
{
}

RenderProgram::~RenderProgram()
{
	GetRenderEngine()->glDeleteProgram(m_ShaderProgram);
	GetRenderEngine()->glDeleteShader(m_VertexShader);
	GetRenderEngine()->glDeleteShader(m_FragmentShader);
}


void RenderProgram::SetProgramEnable(bool enable)
{
	if (enable)
	{
		GetRenderEngine()->UseProgram(GetShaderProgram());
	}
	else
	{
		GetRenderEngine()->UseProgram(0);
	}
}

void RenderProgram::InitShader(const char* vertexShader, const char* fragShader)
{
	m_VertexShader = GetRenderEngine()->CreateShader(vertexShader, GL_VERTEX_SHADER);
	m_FragmentShader = GetRenderEngine()->CreateShader(fragShader, GL_FRAGMENT_SHADER);
	m_ShaderProgram = GetRenderEngine()->CreateProgram(m_VertexShader, m_FragmentShader);

	OnInitShader();
}

void RenderProgram::OnInitShader()
{
}

RenderEngine* RenderProgram::GetRenderEngine()
{
	return RenderEngine::GetInstance();
}

GLuint RenderProgram::GetShaderProgram()
{
	return m_ShaderProgram;
}
