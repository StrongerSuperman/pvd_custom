#pragma once

#include <vector>

#include "core/render/RenderEngine.hpp"
#include "core/render/RenderProgramPhong.hpp"
#include "core/render/RenderObject.hpp"
#include "interface/ICamera.hpp"


class IRenderer
{
public:
	IRenderer();
	~IRenderer();

	void Init();
	void Clear();
	void Render(const RenderObject &object, const ICamera &camera);

protected:
	RenderProgramPhong*  m_RenderProgramPhong;

	virtual void setObjectColor(const RenderObject* object, const ICamera* camera);
	virtual void setLightPosition(const RenderObject* object, const ICamera* camera);
	virtual void setLightColor(const RenderObject* object, const ICamera* camera);

	inline RenderEngine*  GetRenderEngine() const { return RenderEngine::GetInstance(); };

private:
	void render(const RenderObject* object, const ICamera* camera);

	void bindObject(const RenderObject* object);
	void drawObject(const RenderObject* object);
	void sendData(const RenderObject* object, const ICamera* camera);
};
