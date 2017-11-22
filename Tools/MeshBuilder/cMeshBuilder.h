/*
This class builds hardware-ready textures from source images
*/

#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Include Files
//==============

#include <Tools/AssetBuildLibrary/cbBuilder.h>
#include <Engine/Graphics/VertexFormats.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cMeshBuilder : public cbBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			virtual cResult Build(const std::vector<std::string>& i_arguments) override;

			// Winding Order and UVs
			//------
			
			void AdjustWindingOrderAndUVs(uint16_t vertexCount, eae6320::Graphics::VertexFormats::sMesh  * vertexData, uint16_t indexCount, uint16_t * indexData);
		};
	}
}

#endif	// EAE6320_CMESHBUILDER_H
