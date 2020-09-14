#pragma once

#include "RenderProgram.h"


class RenderProgramLine : public RenderProgram
{
public:
	explicit RenderProgramLine();
	virtual ~RenderProgramLine();

	void Render(const RenderObject& object, const Camera& camera) override;

protected:
	void OnInitShader() override;

	void SetVetexAttrEnable(bool enable);

	void SetObjectColor(const glm::vec3& color);
	void SetObjectModelMatrix(const glm::mat4x4& mat);

	void SetCameraViewMatrix(const glm::mat4x4& mat);
	void SetCameraProjectionMatrix(const glm::mat4x4& mat);

private:
	GLint  m_AttributePosition;
	GLint  m_UniformModelMat;
	GLint  m_UniformViewMat;
	GLint  m_UniformProjectionMat;
	GLint  m_UniformColor;
};
