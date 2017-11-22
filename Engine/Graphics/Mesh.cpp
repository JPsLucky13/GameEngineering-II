#include "Mesh.h"

#include "sContext.h"


#include <Engine\Asserts\Asserts.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Logging/Logging.h>
#include <iostream>


// Static Data Initialization
//===========================

eae6320::Assets::cManager<eae6320::Graphics::Mesh> eae6320::Graphics::Mesh::s_manager;

// Implementation
//===============

eae6320::cResult eae6320::Graphics::Mesh::Load(const char* const i_path, Mesh*& o_mesh)
{

	auto result = eae6320::Results::Success;
	Mesh* newMesh = nullptr;
	std::string errorMessage;
	eae6320::Platform::sDataFromFile dataFromFile;
	// Allocate a new mesh
	{
		newMesh = new Mesh();
		if (!newMesh)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
			Logging::OutputError("Failed to allocate memory for the mesh");
			goto OnExit;
		}
	}

	

	//Load the binary data
	if (eae6320::Platform::LoadBinaryFile(i_path, dataFromFile, &errorMessage))
	{
		auto currentOffset = reinterpret_cast<uintptr_t>(dataFromFile.data);
		const auto finalOffset = currentOffset + dataFromFile.size;

		const auto vertexCount = *reinterpret_cast<uint16_t*>(currentOffset);

		currentOffset += sizeof(vertexCount);
		const auto* const vertexArray = reinterpret_cast<VertexFormats::sMesh*>(currentOffset);

		currentOffset += sizeof(VertexFormats::sMesh) * vertexCount;
		const auto indexCount = *reinterpret_cast<uint16_t*>(currentOffset);

		currentOffset += sizeof(indexCount);
		const auto* const indexArray = reinterpret_cast<uint16_t*>(currentOffset);

		if (!(result = newMesh->Initialize(vertexCount, vertexArray, indexCount, indexArray)))
		{
			EAE6320_ASSERTF(false, "Initialization of new texture failed");
			goto OnExit;
		}

	}
	else
	{
		Logging::OutputError("Failed to load the mesh");
		return eae6320::Results::Failure;
	}

	dataFromFile.Free();

OnExit:

	if (result)
	{
		EAE6320_ASSERT(newMesh);
		o_mesh = newMesh;
	}
	else
	{
		if (newMesh)
		{
			newMesh->DecrementReferenceCount();
			newMesh = nullptr;
		}
		o_mesh = nullptr;
	}

	return result;
}


void eae6320::Graphics::Mesh::Delete(eae6320::Graphics::Mesh* &o_mesh)
{
	o_mesh->DecrementReferenceCount();
	o_mesh = nullptr;
}

eae6320::Graphics::Mesh::Mesh()
{

}

eae6320::Graphics::Mesh::~Mesh()
{
	CleanUp();
}



