#pragma once

#include <glm/glm.hpp>


class Ray
{
public:
	Ray();
	Ray(const Ray& ray);
	Ray(const glm::vec3& origin, const glm::vec3& direction);

	inline void SetOrigin(const glm::vec3& origin)       { m_Origin = origin;       };
	inline void SetDirection(const glm::vec3& direction) { m_Direction = direction; };

	inline glm::vec3 GetOrigin() const    { return m_Origin;    };
	inline glm::vec3 GetDirection() const { return m_Direction; };

private:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
};
