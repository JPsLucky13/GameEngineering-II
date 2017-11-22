/*
This file declares the external interface for the effect
*/

#ifndef EAE6320_GRAPHICS_MESH_H
#define EAE6320_GRAPHICS_MESH_H

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
#include <Engine/Assets/cHandle.h>
#include <Engine/Assets/cManager.h>


#if defined( EAE6320_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		class Mesh
		{
			// Interface
			//==========

		public:

			// Access
			//-------

			using Handle = Assets::cHandle<Mesh>;
			static Assets::cManager<Mesh> s_manager;

			static cResult Load(const char* const i_path, Mesh*& o_mesh);

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(Mesh);


			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();


			// Data
			//=====

			// Functions
			//====

			//Delete
			//====
			static void Delete(Mesh* &o_mesh);


			//Bind
			//====
			void Draw();


		private:

			//Data
			//====
			//The index count
			unsigned int m_indexCount;
			void AdjustIndexData();

#if defined( EAE6320_PLATFORM_D3D )
			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
			//A index buffer holds the data for each index
			ID3D11Buffer* m_indexBuffer = nullptr;
			// D3D has an "input layout" object that associates the layout of the vertex format struct
			// with the input from a vertex shader
			ID3D11InputLayout* m_vertexInputLayout = nullptr;
			


#elif defined( EAE6320_PLATFORM_GL )
			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
			// An index buffer holds the data for each index
			GLuint m_indexBufferId = 0;
#endif
			EAE6320_ASSETS_DECLAREREFERENCECOUNT();

			//Initialize and CleanUp
			//====

			//centerPosX and centerPosY define the coordinates of the center of the sprite
			eae6320::cResult Initialize(const uint16_t vertexCount, const eae6320::Graphics::VertexFormats::sMesh * i_vertexData, const uint16_t indexCount, const uint16_t * i_indexData);
			eae6320::cResult CleanUp();

			//Constructor
			Mesh();

			//Destructor
			~Mesh();

		};
	}
}

#endif	// EAE6320_GRAPHICS_SPRITE_H

