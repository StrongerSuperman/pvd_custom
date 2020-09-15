#include "RenderProgramLine.h"

const char* VERTEX_SHADER_CODE_LINE =
{
	"#version 420 \n"
	"in vec3 inPos;\n"
	"\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_Position = projection * view * model * vec4(ingPos, 1.0);\n"
	"}\n"
};

const char* FRAGMENT_SHADER_CODE_LINE =
{
  "#version 420 \n"
	"\n"
	"uniform vec3 objectColor;\n"
	"\n"
	"void main()\n"
	"{\n"
		"fragColor = vec4(objectColor, 1.0);\n"
	"}\n"
};


RenderProgramLine::RenderProgramLine():
	m_AttributePosition(-1),
	m_UniformModelMat(-1),
	m_UniformViewMat(-1),
	m_UniformProjectionMat(-1),
	m_UniformColor(-1)
{
	InitShader(VERTEX_SHADER_CODE_LINE, FRAGMENT_SHADER_CODE_LINE);
}

RenderProgramLine::~RenderProgramLine()
{
}

void RenderProgramLine::Render(const RenderObject& object, const Camera& camera)
{
	SetProgramEnable(true);

	GetRenderEngine()->BindVerticesBuffer(object.RenderBuffer.VerticesBuffer);
	if (object.RenderBuffer.IndicesNum > 0)
	{
		GetRenderEngine()->BindIndicesBuffer(object.RenderBuffer.IndicesBuffer);
	}
	else
	{
		GetRenderEngine()->BindIndicesBuffer(0);
	}

	SetVetexAttrEnable(true);

	SetObjectModelMatrix(object.ModelMatrix);
	SetCameraViewMatrix(camera.GetViewMatrix());
	SetCameraProjectionMatrix(camera.GetProjectionMatrix());

	if (object.RenderBuffer.IndicesNum > 0)
	{
		GetRenderEngine()->DrawElementsLine(object.RenderBuffer.IndicesNum / 2);
	}
	else
	{
		GetRenderEngine()->DrawArraysLine(object.RenderBuffer.VerticesNum / 2);
	}

	SetVetexAttrEnable(false);

	GetRenderEngine()->BindVerticesBuffer(0);
	GetRenderEngine()->BindIndicesBuffer(0);

	SetProgramEnable(false);
}


void RenderProgramLine::SetVetexAttrEnable(bool enable)
{
	GetRenderEngine()->SetPositionAttrEnable(m_AttributePosition, enable);
}

void RenderProgramLine::SetObjectColor(const glm::vec3& color)
{
	GetRenderEngine()->SetVector3f(m_UniformColor, color);
}

void RenderProgramLine::SetObjectModelMatrix(const glm::mat4x4& mat)
{
	GetRenderEngine()->SetMatrix4f(m_UniformModelMat, mat);
}

void RenderProgramLine::SetCameraViewMatrix(const glm::mat4x4& mat)
{
	GetRenderEngine()->SetMatrix4f(m_UniformViewMat, mat);
}

void RenderProgramLine::SetCameraProjectionMatrix(const glm::mat4x4& mat)
{
	GetRenderEngine()->SetMatrix4f(m_UniformProjectionMat, mat);
}


void RenderProgramLine::OnInitShader()
{
	m_AttributePosition = GetRenderEngine()->glGetAttribLocation(GetShaderProgram(), "inPos");
	m_UniformModelMat = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "model");
	m_UniformViewMat = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "view");
	m_UniformProjectionMat = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "projection");
	m_UniformColor = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "objectColor");
}
