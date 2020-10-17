#pragma once

#include "IRenderer.h"


class Renderer : public IRenderer
{
public:
	Renderer();
	virtual ~Renderer();

	inline void             SetPickedRenderObjectIds(const std::vector<int>& ids) { m_PickedObjIds = ids; };
	inline std::vector<int> GetPickedRenderObjectsIds() { return m_PickedObjIds; };

	virtual void setObjectColor(const RenderObject* object, const ICamera* camera) override;
	//virtual void setLightPosition(const RenderObject* object, const ICamera* camera) override;
	//virtual void setLightColor(const RenderObject* object, const ICamera* camera) override;

private:
	std::vector<int>         m_PickedObjIds;

};
