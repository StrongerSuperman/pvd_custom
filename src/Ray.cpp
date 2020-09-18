#include "Ray.h"


Ray::Ray() :
	m_Origin(glm::vec3(0, 0, 0)),
	m_Direction(glm::vec3(0, 0, -1))
{
}

Ray::Ray(const Ray& ray)
{
	m_Origin = ray.GetOrigin();
	m_Direction = ray.GetDirection();
}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
{
	m_Origin = origin;
	m_Direction = direction;
}
