#pragma once

#include "core/render/RenderProgramBase.hpp"


class RenderProgramPhong : public RenderProgramBase
{
public:
	explicit RenderProgramPhong();
	virtual ~RenderProgramPhong();

	void SetPNAttrEnable(bool enable);
	void SetPAttrEnable(bool enable);

	void SetObjectColor(const glm::vec3& color);
	void SetObjectModelMatrix(const glm::mat4x4& mat);

	void SetCameraPosition(const glm::vec3& position);
	void SetCameraViewMatrix(const glm::mat4x4& mat);
	void SetCameraProjectionMatrix(const glm::mat4x4& mat);

	void SetLightPosition(const glm::vec3& pos);
	void SetLightColor(const glm::vec3& color);

protected:
	void OnInitShader() override;

private:
	GLint  m_AttributePosition;
	GLint  m_AttributeNormal;
	GLint  m_AttributeColor;
	GLint  m_UniformModelMat;
	GLint  m_UniformViewMat;
	GLint  m_UniformProjectionMat;
	GLint  m_UniformColor;
	GLint  m_UniformViewPosition;
	GLint  m_UniformLightPosition;
	GLint  m_UniformLightColor;
};
