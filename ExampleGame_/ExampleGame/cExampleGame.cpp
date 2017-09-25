// Include Files
//==============

#include "cExampleGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>



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

	//Render the sprite pairs

	for (size_t i = 0; i < sprites.size(); i++)
	{
		eae6320::Graphics::RenderSpriteWithEffect(sprites[i],effects[i]);
	}
	

	//User specify's the background clear color
	eae6320::Graphics::ClearColor(0.5f,0.0f,0.0f,1.0f);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cExampleGame::Initialize()
{
	eae6320::Graphics::Effect * newEffect;
	eae6320::Graphics::Sprite * newSprite;


	//Call to factory function for effects
	eae6320::Graphics::Effect::Factory(newEffect, "sprite", "sprite1", eae6320::Graphics::RenderStates::AlphaTransparency);
	effects.push_back(newEffect);
	eae6320::Graphics::Effect::Factory(newEffect, "sprite", "sprite2", eae6320::Graphics::RenderStates::AlphaTransparency);
	effects.push_back(newEffect);
	eae6320::Graphics::Effect::Factory(newEffect, "sprite", "sprite2", eae6320::Graphics::RenderStates::AlphaTransparency);
	effects.push_back(newEffect);

	//Call to factory function for sprites
	eae6320::Graphics::Sprite::Factory(newSprite, 0.5f, 0.5f, 0.5f, 0.5f);
	sprites.push_back(newSprite);
	eae6320::Graphics::Sprite::Factory(newSprite, -0.5f, -0.5f, 1.0f, 1.0f);
	sprites.push_back(newSprite);
	eae6320::Graphics::Sprite::Factory(newSprite, -0.5f, 0.5f, 0.25f, 0.25f);
	sprites.push_back(newSprite);

	return Results::Success;
}

eae6320::cResult eae6320::cExampleGame::CleanUp()
{
	//Destroy the effects
	for (size_t i = 0; i < effects.size(); i++)
	{
		eae6320::Graphics::Effect::Delete(effects[i]);
	}
	//Destroy the sprites
	for (size_t i = 0; i < sprites.size(); i++)
	{
		eae6320::Graphics::Sprite::Delete(sprites[i]);
	}

	return Results::Success;
}
