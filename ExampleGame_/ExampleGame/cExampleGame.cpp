// Include Files
//==============

#include "cExampleGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Graphics/Graphics.h>


// Inherited Implementation
//=========================

// Run
//----

void eae6320::cExampleGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	// Effects
	//-------------
	eae6320::Graphics::Effect * effect1;
	//eae6320::Graphics::Effect * effect2;

	// Sprites
	//--------------
	eae6320::Graphics::Sprite * sprite1;
	//eae6320::Graphics::Sprite * sprite2;
	//eae6320::Graphics::Sprite * sprite3;

	//Call to factory function for effects
	eae6320::Graphics::Effect::Factory(effect1, "sprite", "sprite1", eae6320::Graphics::RenderStates::AlphaTransparency);
	//eae6320::Graphics::Effect::Factory(effect2, "sprite", "sprite2", eae6320::Graphics::RenderStates::AlphaTransparency);

	//Call to factory function for sprites
	eae6320::Graphics::Sprite::Factory(sprite1, 0.5f, 0.5f, 0.5f, 0.5f);
	//eae6320::Graphics::Sprite::Factory(sprite2, -0.5f, -0.5f, 1.0f, 1.0f);
	//eae6320::Graphics::Sprite::Factory(sprite3, -0.5f, 0.5f, 0.25f, 0.25f);

	//Increment reference count for each effect and sprite
	effect1->IncrementReferenceCount();
	//effect2->IncrementReferenceCount();
	sprite1->IncrementReferenceCount();
	//sprite2->IncrementReferenceCount();
	//sprite3->IncrementReferenceCount();

	//Render the sprite pairs
	//eae6320::Graphics::RenderSpriteWithEffect(*sprite1,*effect1,1);
	//eae6320::Graphics::RenderSpriteWithEffect(*sprite2, *effect2, 1);
	//eae6320::Graphics::RenderSpriteWithEffect(*sprite3, *effect2, 1);

	//User specify's the background clear color
	eae6320::Graphics::ClearColor(0.5f,0.0f,0.0f,1.0f);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cExampleGame::Initialize()
{
	return Results::Success;
}

eae6320::cResult eae6320::cExampleGame::CleanUp()
{
	return Results::Success;
}
