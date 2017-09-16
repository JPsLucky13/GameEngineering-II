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

#include "sContext.h"

#include <cstdint>
#include <Engine/Results/Results.h>
#include <Engine/Assets/ReferenceCountedAssets.h>


#if defined( EAE6320_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		class Effect
		{
			// Interface
			//==========

		public:

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()


			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(Effect);

			// Data
			//=====




			// Functions
			//====

			//Initialize
			//====
			static cResult Factory(Effect* &o_effect, char * vertexShaderName, char * fragmentShaderName, uint8_t defaultRenderState);

			//Bind
			//====
			void Bind();

			void BindRenderState();

			

			//Create Program
			eae6320::cResult CreateProgram(eae6320::cResult & result);

			//Check Program Id
			eae6320::cResult CheckProgramID(eae6320::cResult & result);

		private:

			// Data
			//=====

#if defined( EAE6320_PLATFORM_GL )
			GLuint m_programId = 0;
#endif

			eae6320::Graphics::cShader::Handle m_vertexShader;
			eae6320::Graphics::cShader::Handle m_fragmentShader;

			eae6320::Graphics::cRenderState m_renderState;
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()

			//Constructor
			Effect();

			//Destructor
			~Effect();

			//Initialize and CleanUp
			//====
			eae6320::cResult Initialize(char * vertexShaderName, char * fragmentShaderName, uint8_t defaultRenderState);
			eae6320::cResult CleanUp();


		};
	}
}


