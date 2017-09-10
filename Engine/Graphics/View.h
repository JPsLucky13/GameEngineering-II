/*
This file declares the external interface for the effect
*/

// Include Files
//==============
#include "cShader.h"
#include "cRenderState.h"
#include "sContext.h"

#if defined( EAE6320_PLATFORM_D3D )
#include "Direct3D\Includes.h"
#endif

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
		struct View
		{
			// Interface
			//==========

		public:

			// Data
			//=====


			// Functions
			//====
			void GetContext();

			eae6320::cResult InitializeViews(const sInitializationParameters& i_initializationParameters);

			void CleanUp();

			void ClearColor(float red, float green, float blue, float alpha);

			void ViewSwapBuffers();

		private:
			// Data
			//=====
#if defined( EAE6320_PLATFORM_D3D )
			ID3D11DeviceContext* m_direct3dContext;

			// In Direct3D "views" are objects that allow a texture to be used a particular way:
			// A render target view allows a texture to have color rendered to it
			ID3D11RenderTargetView* m_renderTargetView = nullptr;
			// A depth/stencil view allows a texture to have depth rendered to it
			ID3D11DepthStencilView* m_depthStencilView = nullptr;
#endif
		};
	}
}
