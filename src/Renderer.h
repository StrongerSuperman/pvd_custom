#pragma once

#include <vector>

#include "RenderEngine.h"
#include "RenderProgramPhong.h"
#include "RenderObject.h"
#include "Camera.h"


class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init();

	void Render(std::vector<RenderObject> &objects, Camera &camera);
	void RenderLine(std::vector<RenderObject> &objects, Camera &camera);

	inline void             SetPickedRenderObjectIds(const std::vector<int>& ids) { m_PickedObjIds = ids; };
	inline std::vector<int> GetPickedRenderObjectsIds() { return m_PickedObjIds; };

private:
	std::vector<int>         m_PickedObjIds;

	RenderProgramPhong*      m_RenderProgramPhong;

	void render(RenderObject* objects, Camera* camera);
	void renderLine(RenderObject* objects, Camera* camera);

	inline RenderEngine*  GetRenderEngine() const { return RenderEngine::GetInstance(); };
	void bindObject(RenderObject* object);
	void setShaderUniform(RenderObject* object, Camera* camera);
};
