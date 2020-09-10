#pragma once

#include "RenderFunc.h"
#include "RenderObject.h"
#include "Camera.h"


class RenderProgram
{
public:
	explicit RenderProgram();
	virtual ~RenderProgram();

	virtual void Init() = 0;
	virtual void Render(const RenderObject& object, const Camera& camera) = 0;

	inline virtual void SetPickedRenderObject(int id) { m_PickedRenderObjectId = id;   };
	inline int          GetPickedRenderObject()       { return m_PickedRenderObjectId; };

protected:
	virtual void OnInitShader();
	void         InitShader(const char* vertexShader, const char* fragShader);

	void         SetProgramEnable(bool enable);
	RenderFunc*  GetRenderFunc();
	GLuint       GetShaderProgram();
	
	int          m_PickedRenderObjectId;
private:
	GLuint       m_VertexShader;
	GLuint       m_FragmentShader;
	GLuint       m_ShaderProgram;
};
