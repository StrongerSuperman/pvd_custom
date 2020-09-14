#pragma once

#include "RenderFunc.h"
#include "RenderObject.h"
#include "Camera.h"


class RenderProgram
{
public:
	explicit RenderProgram();
	virtual ~RenderProgram();

	virtual void Render(const RenderObject& object, const Camera& camera) = 0;

	inline void             SetPickedRenderObjects(const std::vector<int>& ids) { m_PickedRenderObjectIds = ids;  };
	inline std::vector<int> GetPickedRenderObjects()                            { return m_PickedRenderObjectIds; };

protected:
	virtual void       OnInitShader();
	void               InitShader(const char* vertexShader, const char* fragShader);

	void               SetProgramEnable(bool enable);
	RenderFunc*        GetRenderFunc();
	GLuint             GetShaderProgram();
	
	std::vector<int>   m_PickedRenderObjectIds;

private:
	GLuint       m_VertexShader;
	GLuint       m_FragmentShader;
	GLuint       m_ShaderProgram;
};
