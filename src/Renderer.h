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
	void Render(const std::vector<RenderObject> &objects, const Camera &camera);

	void ClearBuffer();

	inline void             SetPickedRenderObjectIds(const std::vector<int>& ids) { m_PickedObjIds = ids; };
	inline std::vector<int> GetPickedRenderObjectsIds() { return m_PickedObjIds; };

private:
	std::vector<int>         m_PickedObjIds;
	RenderProgramPhong*      m_RenderProgramPhong;

	void render(const RenderObject* objects, const Camera* camera);

	void bindObject(const RenderObject* object);
	void drawObject(const RenderObject* object);
	void sendData(const RenderObject* object, const Camera* camera);

	inline RenderEngine*  GetRenderEngine() const { return RenderEngine::GetInstance(); };
};
