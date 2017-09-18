#include "Sprite.h"

#include "sContext.h"
#include "VertexFormats.h"


#include <Engine\Asserts\Asserts.h>
#include <Engine/Platform/Platform.h>

// Implementation
//===============

eae6320::cResult eae6320::Graphics::Sprite::Factory(Sprite* &o_sprite, float centerPosX, float centerPosY, float width, float height)
{
	auto result = Results::Success;

	Sprite* newSprite = nullptr;

	// Allocate a new sprite
	{
		newSprite = new Sprite();
		if (!newSprite)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the sprite");
			Logging::OutputError("Failed to allocate memory for the sprite");
			goto OnExit;
		}
	}

	// Initialize the platform-specific graphics API sprite object
	if (!(result = newSprite->Initialize(centerPosX, centerPosY, width,height)))
	{
		EAE6320_ASSERTF(false, "Initialization of new sprite failed");
		goto OnExit;
	}

OnExit:

	if (result)
	{
		EAE6320_ASSERT(newSprite);
		o_sprite = newSprite;
	}
	else
	{
		if (newSprite)
		{
			newSprite->DecrementReferenceCount();
			newSprite = nullptr;
		}
		o_sprite = nullptr;
	}

	return result;

}

void eae6320::Graphics::Sprite::Delete(eae6320::Graphics::Sprite* &o_sprite)
{
	o_sprite->DecrementReferenceCount();
	o_sprite = nullptr;
}

eae6320::Graphics::Sprite::Sprite()
{

}

eae6320::Graphics::Sprite::~Sprite()
{
	CleanUp();
}