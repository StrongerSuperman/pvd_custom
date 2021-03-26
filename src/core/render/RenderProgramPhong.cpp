#include "core/render/RenderProgramPhong.hpp"

const char* VERTEX_SHADER_CODE_PHONG =
{
	"#version 420 \n"
	"in vec3 inPos;\n"
	"in vec3 inNormal;\n"
	"out vec3 fragPos;\n"
	"out vec3 normal;\n"
	"\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"\n"
	"void main()\n"
	"{\n"
	"\n"
	"   normal = vec3(transpose(inverse(model)) * vec4(inNormal, 1.0));\n"
	"   fragPos = vec3(model * vec4(inPos, 1.0));\n"
#ifndef COORDINATE_RIGHT_HANDED
	"   normal = normalize(vec3(normal.x, normal.y, -normal.z));\n"
	"   fragPos = vec3(fragPos.x, fragPos.y, -fragPos.z);\n"
#endif
	"   gl_Position = projection * view * vec4(fragPos, 1.0);\n"
	"}\n"
};

const char* FRAGMENT_SHADER_CODE_PHONG =
{
	"#version 420 \n"
	"in vec3 fragPos;\n"
	"in vec3 normal;\n"
	"out vec4 fragColor;\n"
	"\n"
	"uniform vec3 objectColor;\n"
	"uniform vec3 viewPos;\n"
	"uniform vec3 lightPos;\n"
	"uniform vec3 lightColor;\n"
	"\n"
	"void main()\n"
	"{\n"
		"// ambient \n"
		"float ambientStrength = 0.2;\n"
		"vec3 ambient = ambientStrength * lightColor;\n"
		"\n"
		"// diffuse \n"
		"vec3 lightDir = normalize(lightPos - fragPos);\n"
		"float diff = max(dot(normal, lightDir), 0.0);\n"
		"vec3 diffuse = diff * lightColor;\n"
		"\n"
		"// specular\n"
		"float specularStrength = 0.3;\n"
		"vec3 viewDir = normalize(viewPos - fragPos);\n"
		"vec3 halfDir = normalize(lightDir + viewDir);\n"
		"float spec = pow(max(dot(viewDir, halfDir), 0.0), 32);\n"
		"vec3 specular = specularStrength * spec * lightColor;\n"
		"\n"
		"vec3 result = (ambient + diffuse + specular) * objectColor;\n"
		"fragColor = vec4(result, 1.0);\n"
	"}\n"
};


RenderProgramPhong::RenderProgramPhong():
	m_AttributePosition(-1),
	m_AttributeNormal(-1),
	m_AttributeColor(-1),
	m_UniformModelMat(-1),
	m_UniformViewMat(-1),
	m_UniformProjectionMat(-1),
	m_UniformColor(-1),
	m_UniformViewPosition(-1),
	m_UniformLightPosition(-1),
	m_UniformLightColor(-1)
{
	InitShader(VERTEX_SHADER_CODE_PHONG, FRAGMENT_SHADER_CODE_PHONG);
}

RenderProgramPhong::~RenderProgramPhong()
{
}


void RenderProgramPhong::SetPNAttrEnable(bool enable)
{
	GetRenderEngine()->SetPNAttrEnable(m_AttributePosition, m_AttributeNormal, enable);
}

void RenderProgramPhong::SetPAttrEnable(bool enable)
{
	GetRenderEngine()->SetPAttrEnable(m_AttributePosition, enable);
}


void RenderProgramPhong::SetObjectColor(const glm::vec3& color)
{
	GetRenderEngine()->SetVector3f(m_UniformColor, color);
}

void RenderProgramPhong::SetObjectModelMatrix(const glm::mat4x4& mat)
{
	GetRenderEngine()->SetMatrix4f(m_UniformModelMat, mat);
}

void RenderProgramPhong::SetCameraPosition(const glm::vec3& pos)
{
	GetRenderEngine()->SetVector3f(m_UniformViewPosition, pos);
}

void RenderProgramPhong::SetCameraViewMatrix(const glm::mat4x4& mat)
{
	GetRenderEngine()->SetMatrix4f(m_UniformViewMat, mat);
}

void RenderProgramPhong::SetCameraProjectionMatrix(const glm::mat4x4& mat)
{
	GetRenderEngine()->SetMatrix4f(m_UniformProjectionMat, mat);
}

void RenderProgramPhong::SetLightPosition(const glm::vec3& pos)
{
	GetRenderEngine()->SetVector3f(m_UniformLightPosition, pos);
}

void RenderProgramPhong::SetLightColor(const glm::vec3& color)
{
	GetRenderEngine()->SetVector3f(m_UniformLightColor, color);
}


void RenderProgramPhong::OnInitShader()
{
	m_AttributePosition = GetRenderEngine()->glGetAttribLocation(GetShaderProgram(), "inPos");
	m_AttributeNormal = GetRenderEngine()->glGetAttribLocation(GetShaderProgram(), "inNormal");
	m_UniformModelMat = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "model");
	m_UniformViewMat = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "view");
	m_UniformProjectionMat = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "projection");
	m_UniformColor = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "objectColor");
	m_UniformViewPosition = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "viewPos");
	m_UniformLightPosition = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "lightPos");
	m_UniformLightColor = GetRenderEngine()->glGetUniformLocation(GetShaderProgram(), "lightColor");
}
