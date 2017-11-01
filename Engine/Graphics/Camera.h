/*
This file declares the external interface for the effect
*/

#ifndef EAE6320_GRAPHICS_CAMERA_H
#define EAE6320_GRAPHICS_CAMERA_H

// Include Files
//==============

#include "VertexFormats.h"

#if defined( EAE6320_PLATFORM_GL )
#include "OpenGL\Includes.h"
#endif

#if defined( EAE6320_PLATFORM_D3D )
#include "Direct3D\Includes.h"
#endif

#include <cstdint>
#include <Engine/Results/Results.h>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Physics/sRigidBodyState.h>


#if defined( EAE6320_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		class Camera
		{
			// Interface
			//==========

		public:
			// Data
			//=====
			Physics::sRigidBodyState m_cameraRigidBody;
			float m_verticalFieldOfView_inRadians;
			float m_aspectRatio;
			float m_z_nearPlane;
			float m_z_farPlane;

			//Constructor
			Camera(Physics::sRigidBodyState cameraRigidBody, const float i_verticalFieldOfView_inRadians, const float i_aspectRatio, const float i_z_nearPlane, const float i_z_farPlane);
			//Destructor
			~Camera();

		};
	}
}

#endif	// EAE6320_GRAPHICS_CAMERA_H

