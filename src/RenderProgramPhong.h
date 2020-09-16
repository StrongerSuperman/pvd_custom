#pragma once

#include "RenderProgram.h"


class RenderProgramPhong : public RenderProgram
{
public:
	explicit RenderProgramPhong();
	virtual ~RenderProgramPhong();

	virtual void Render(const RenderObject& object, const Camera& camera) = 0;

protected:
	void OnInitShader() override;

	void SetObjectColor(const glm::vec3& color);
	void SetObjectModelMatrix(const glm::mat4x4& mat);

	void SetCameraPosition(const glm::vec3& position);
	void SetCameraViewMatrix(const glm::mat4x4& mat);
	void SetCameraProjectionMatrix(const glm::mat4x4& mat);

	void SetLightPosition(const glm::vec3& pos);
	void SetLightColor(const glm::vec3& color);

	inline GLint GetAttributePosition() const { return m_AttributePosition; };
	inline GLint GetAttributeNormal() const { return m_AttributeNormal; };
	inline GLint GetAttributeColor() const { return m_AttributeColor; };

	void StartProgram(const RenderObject& object);
	void SetShaderUniform(const RenderObject& object, const Camera& camera);
	void EndProgram();

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
