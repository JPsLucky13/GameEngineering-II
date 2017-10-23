#include "Mesh.h"

#include "sContext.h"


#include <Engine\Asserts\Asserts.h>
#include <Engine/Platform/Platform.h>

// Implementation
//===============

eae6320::cResult eae6320::Graphics::Mesh::Factory(Mesh* &o_mesh, eae6320::Graphics::VertexFormats::sMesh vertexData[], uint16_t indexData[])
{
	auto result = Results::Success;

	Mesh* newMesh = nullptr;

	// Allocate a new sprite
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

	// Initialize the platform-specific graphics API sprite object
	if (!(result = newMesh->Initialize(vertexData, indexData)))
	{
		EAE6320_ASSERTF(false, "Initialization of new mesh failed");
		goto OnExit;
	}

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



