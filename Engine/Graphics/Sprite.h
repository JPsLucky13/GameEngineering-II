/*
This file declares the external interface for the effect
*/

// Include Files
//==============
#include "cShader.h"
#include "cRenderState.h"

#if defined( EAE6320_PLATFORM_GL )
#include "OpenGL\Includes.h"
#endif

#if defined( EAE6320_PLATFORM_D3D )
#include "Direct3D\Includes.h"
#endif

#include "sContext.h"

#include <cstdint>
#include <Engine/Results/Results.h>


#if defined( EAE6320_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Sprite
		{
			// Interface
			//==========

		public:

			// Data
			//=====

			// Functions
			//====

			// Get
			//====
			void GetContext();

			//Initialize
			//====

			//centerPosX and centerPosY define the coordinates of the center of the sprite
			eae6320::cResult Initialize(float centerPosX, float centerPosY, float width, float height);

			//Bind
			//====
			void Draw();

			//CleanUp
			//====
			void CleanUp(eae6320::cResult & result);


		private:

			//Data
			//====
#if defined( EAE6320_PLATFORM_D3D )
			ID3D11DeviceContext* m_direct3dContext;
			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
			// D3D has an "input layout" object that associates the layout of the vertex format struct
			// with the input from a vertex shader
			ID3D11InputLayout* m_vertexInputLayout = nullptr;


#elif defined( EAE6320_PLATFORM_GL )
			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
#endif

		};
	}
}


