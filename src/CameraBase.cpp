#include "CameraBase.h"


CameraBase::CameraBase():
	//m_ProjectionMode(ProjectionMode::Orthogonal),
	m_ProjectionMode(ProjectionMode::Perspective),

	m_Fov(45.0f),
	m_Aspect(1.0f),
	m_NearZ(0.1f),
	m_FarZ(100000.0f),
	
	m_Left(-100.0f),
	m_Right(100.0f),
	m_Bottom(-100.0f),
	m_Top(100.0f),
	m_NearPlane(0.1f),
	m_FarPlane(100000.0f),

	m_Eye(glm::vec3(0.0f, 0.0f, 3.0f)),
	m_Dir(-glm::normalize(m_Eye)),

	m_ViewMatrix(glm::mat4x4()),
	m_ProjectionMatrix(glm::mat4x4())
{
	UpdateMatrix();
}

CameraBase::~CameraBase()
{
}


void CameraBase::SetEyeAndDir(glm::vec3& eye, glm::vec3& dir)
{
	m_Eye = eye;
	m_Dir = dir;

	UpdateViewMatrix();
}

void CameraBase::SetEye(glm::vec3& eye)
{
	m_Eye = eye;
	UpdateViewMatrix();
}

void CameraBase::SetDir(glm::vec3& dir)
{
	m_Dir = dir;
	UpdateViewMatrix();
}


void CameraBase::SetPerspectiveMode()
{
	m_ProjectionMode = ProjectionMode::Perspective;
}

void CameraBase::SetOrthogonalMode()
{
	m_ProjectionMode = ProjectionMode::Orthogonal;
}


void CameraBase::SetFov(float fov)
{
	m_Fov = fov;
	UpdateProjectionMatrix();
}

void CameraBase::SetAspectRatio(float aspect)
{
	m_Aspect = aspect;
	UpdateProjectionMatrix();
}

void CameraBase::SetOrthgonalMode()
{
	m_ProjectionMode = ProjectionMode::Orthogonal;
}

void CameraBase::SetOrthgonalData(const glm::vec3 &minimum, const glm::vec3 &maximum)
{
	m_Left = -(maximum.x - minimum.x)*2;
	m_Right = (maximum.x - minimum.x) * 2;
	m_Bottom = -(maximum.y - minimum.y) * 2;
	m_Top = (maximum.y - minimum.y) * 2;
	UpdateProjectionMatrix();
}


void CameraBase::SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	m_ViewPort.TopLeftX = topLeftX;
	m_ViewPort.TopLeftY = topLeftY;
	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = minDepth;
	m_ViewPort.MaxDepth = maxDepth;
}


void CameraBase::UpdateMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraBase::UpdateViewMatrix()
{
	glm::vec3 right = glm::normalize(glm::cross(m_Dir, glm::vec3(0, 1, 0)));
	glm::vec3 up = glm::normalize(glm::cross(right, m_Dir));
	m_ViewMatrix = glm::lookAt(m_Eye, m_Eye + m_Dir, up);
}

void CameraBase::UpdateProjectionMatrix()
{
	if (m_ProjectionMode == ProjectionMode::Perspective)
	{
		m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, m_NearZ, m_FarZ);
	}
	else
	{
		m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearPlane, m_FarPlane);
	}
}
