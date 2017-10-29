#include "Camera.h"

#include "sContext.h"


#include <Engine\Asserts\Asserts.h>
#include <Engine/Platform/Platform.h>

// Implementation
//===============

eae6320::Graphics::Camera::Camera(Physics::sRigidBodyState p_cameraRigidBody, const float p_verticalFieldOfView_inRadians, const float p_aspectRatio, const float p_z_nearPlane, const float p_z_farPlane) :
	m_cameraRigidBody(p_cameraRigidBody),
	m_verticalFieldOfView_inRadians(p_verticalFieldOfView_inRadians),
	m_aspectRatio(p_aspectRatio),
	m_z_nearPlane(p_z_nearPlane),
	m_z_farPlane(p_z_farPlane)
{
}

eae6320::Graphics::Camera::~Camera()
{
}
