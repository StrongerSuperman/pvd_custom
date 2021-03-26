#pragma once

#include "core/render/RenderEngine.hpp"


class RenderProgramBase
{
public:
	explicit RenderProgramBase();
	virtual ~RenderProgramBase();

	void                  SetProgramEnable(bool enable);

protected:
	virtual void          OnInitShader();
	void                  InitShader(const char* vertexShader, const char* fragShader);
	
	inline GLuint         GetShaderProgram() const { return m_ShaderProgram; };
	inline RenderEngine*  GetRenderEngine() const { return RenderEngine::GetInstance(); };

private:
	GLuint       m_VertexShader;
	GLuint       m_FragmentShader;
	GLuint       m_ShaderProgram;
};
