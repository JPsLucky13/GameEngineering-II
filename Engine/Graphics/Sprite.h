/*
This file declares the external interface for the effect
*/

#ifndef EAE6320_GRAPHICS_SPRITE_H
#define EAE6320_GRAPHICS_SPRITE_H

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


#if defined( EAE6320_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		class Sprite
		{
			// Interface
			//==========

		public:
			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(Sprite);

			
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();


			// Data
			//=====

			// Functions
			//====

			//Factory
			//====
			static cResult Factory(Sprite* &o_sprite, float centerPosX, float centerPosY, float width, float height);

			//Delete
			//====
			static void Delete(Sprite* &o_sprite);


			//Bind
			//====
			void Draw();


		private:

			//Data
			//====

#if defined( EAE6320_PLATFORM_D3D )
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
			EAE6320_ASSETS_DECLAREREFERENCECOUNT();

			//Initialize and CleanUp
			//====

			//centerPosX and centerPosY define the coordinates of the center of the sprite
			eae6320::cResult Initialize(float centerPosX, float centerPosY, float width, float height);
			eae6320::cResult CleanUp();

			//Constructor
			Sprite();

			//Destructor
			~Sprite();

			void CalculateXYRemainingVertices(float centerPosX, float centerPosY, float width, float height,eae6320::Graphics::VertexFormats::sSprite vertexData[]);
			void CalculateUVRemainingVertices(eae6320::Graphics::VertexFormats::sSprite vertexData[]);

		};
	}
}

#endif	// EAE6320_GRAPHICS_SPRITE_H

