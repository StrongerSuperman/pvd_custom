#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CameraBase
{
	enum ProjectionMode
	{
		Perspective,
		Orthogonal,
	};

	struct ViewPort
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

public:
	CameraBase();
	virtual ~CameraBase();

	void                   SetEyeAndDir(glm::vec3& eye, glm::vec3& dir);
	void                   SetEye(glm::vec3& eye);
	void                   SetDir(glm::vec3& dir);

	void                   SetPerspectiveMode();
	void                   SetOrthogonalMode();

	void                   SetFov(float fov);
	void                   SetAspectRatio(float aspect);
	void                   SetOrthgonalMode();
	void                   SetOrthgonalData(const glm::vec3 &minimum, const glm::vec3 &maximum);

	void                   SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);
					       
	inline glm::vec3	   GetEye()	                 const { return m_Eye; };
	inline glm::vec3	   GetDir()	                 const { return m_Dir; };

	inline float           GetFov()                  const { return m_Fov; };
	inline float           GetAspectRatio()          const { return m_Aspect; };

	inline ViewPort        GetViewPort()             const { return m_ViewPort; };

	inline glm::mat4x4	   GetViewMatrix()           const { return m_ViewMatrix; };
	inline glm::mat4x4	   GetProjectionMatrix()     const { return m_ProjectionMatrix; };
	inline glm::mat4x4     GetProjectionViewMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); };
					       
protected:
	void                   UpdateMatrix();
	void                   UpdateViewMatrix();
	void                   UpdateProjectionMatrix();

private:
	ProjectionMode         m_ProjectionMode;

	/*perspective projection data*/
	float                  m_Fov;
	float                  m_Aspect;
	float                  m_NearZ;
	float                  m_FarZ;

	/*orthgonal projection data*/
	float                  m_Left;
	float                  m_Right;
	float                  m_Bottom;
	float                  m_Top;
	float                  m_NearPlane;
	float                  m_FarPlane;

	glm::vec3	           m_Eye;
	glm::vec3	           m_Dir;
	ViewPort               m_ViewPort;
	glm::mat4x4            m_ViewMatrix;
	glm::mat4x4            m_ProjectionMatrix;
};
