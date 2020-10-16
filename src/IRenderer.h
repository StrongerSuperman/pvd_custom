#pragma once

#include <vector>

#include "RenderEngine.h"
#include "RenderProgramPhong.h"
#include "RenderObject.h"
#include "ICamera.h"


class IRenderer
{
public:
	IRenderer();
	~IRenderer();

	void Init();
	void Clear();
	void Render(const RenderObject &object, const ICamera &camera);

protected:
	RenderProgramPhong*      m_RenderProgramPhong;

	virtual void setObjectColor(const RenderObject* object);

	inline RenderEngine*  GetRenderEngine() const { return RenderEngine::GetInstance(); };

private:
	void render(const RenderObject* object, const ICamera* camera);

	void bindObject(const RenderObject* object);
	void drawObject(const RenderObject* object);
	void sendData(const RenderObject* object, const ICamera* camera);
};
