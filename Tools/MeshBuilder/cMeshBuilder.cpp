// Include Files
//==============

#include "cMeshBuilder.h"
#include <Engine\Platform\Platform.h>
#include <Tools\AssetBuildLibrary\Functions.h>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>&)
{
	auto result = eae6320::Results::Success;

	// Copy the file
	{
		std::string errorMessage;
		// There are many reasons that a source should be rebuilt,
		// and so even if the target already exists it should just be written over
		constexpr bool noErrorIfTargetAlreadyExists = false;
		// Since we rely on timestamps to determine when a target was built
		// its file time should be updated when the source gets copied
		constexpr bool updateTheTargetFileTime = true;
		if (eae6320::Platform::CopyFile(m_path_source, m_path_target, noErrorIfTargetAlreadyExists, updateTheTargetFileTime, &errorMessage))
		{
			return result;	// Return a boolean [true]
		}
		else
		{
			OutputErrorMessageWithFileInfo(m_path_source, errorMessage.c_str());
			return eae6320::Results::Failure;
		}
	}
}