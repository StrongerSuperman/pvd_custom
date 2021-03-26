#pragma once

#include <vector>

#include <glm/vec3.hpp>

#include "core/scene/SceneObject.hpp"


class IScene
{
public:
	IScene();
	virtual ~IScene() = 0;

	virtual void Render() = 0;

protected:

	std::vector<SceneObject>     m_SceneObjects;
};
