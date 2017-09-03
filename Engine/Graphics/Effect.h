/*
This file declares the external interface for the effect
*/

// Include Files
//==============
#include "cShader.h"
#include "cRenderState.h"
#include "OpenGL\Includes.h"
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
		struct Effect
		{
			// Interface
			//==========

		public:

			// Data
			//=====

			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;
#if defined( EAE6320_PLATFORM_GL )
			GLuint m_programId = 0;
#endif
			eae6320::Graphics::cRenderState m_renderState;
#if defined( EAE6320_PLATFORM_D3D )
			eae6320::Graphics::sContext * m_currentContext;
#endif

			// Functions
			//====


			//Initialize
			//====
			eae6320::cResult Initialize();

			//Bind
			//====
			void Bind();

			//CleanUp
			//====
			void CleanUp(eae6320::cResult & result);


		private:

		};
	}
}


