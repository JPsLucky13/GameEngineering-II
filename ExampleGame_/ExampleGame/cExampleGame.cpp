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

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{

		if (!isKeyPressedAlready)
		{
			isKeyPressedAlready = true;

			// Exit the application
			eae6320::Graphics::cTexture::Handle storedTexture = textures[0];
			textures[0] = textures[1];
			textures[1] = storedTexture;
		}
		
	}
	
	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		isKeyPressedAlready = false;
	}
}

void eae6320::cExampleGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{

	counter += i_elapsedSecondCount_sinceLastUpdate;

	if (counter > timeToSwitchTexture)
	{
		eae6320::Graphics::cTexture::Handle storedTexture = textures[2];
		textures[2] = textures[3];
		textures[3] = storedTexture;
		counter = 0.0f;

	}

}


void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{

	//Render the sprite pairs
	for (size_t i = 0; i < sprites.size(); i++)
	{
		eae6320::Graphics::RenderSpriteWithEffectAndTexture(sprites[i],effects[i],textures[i]);
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
	auto result = eae6320::Graphics::Effect::Factory(newEffect, "sprite", "sprite1", eae6320::Graphics::RenderStates::AlphaTransparency);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	
	effects.push_back(newEffect);
	
	result = eae6320::Graphics::Effect::Factory(newEffect, "sprite", "sprite2", eae6320::Graphics::RenderStates::AlphaTransparency);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	
	effects.push_back(newEffect);


	result = eae6320::Graphics::Effect::Factory(newEffect, "sprite", "sprite2", eae6320::Graphics::RenderStates::AlphaTransparency);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}

	effects.push_back(newEffect);

	//Call to factory function for sprites
	result = eae6320::Graphics::Sprite::Factory(newSprite, 0.5f, 0.5f, 0.5f, 0.5f);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	sprites.push_back(newSprite);
	result = eae6320::Graphics::Sprite::Factory(newSprite, -0.5f, -0.5f, 1.0f, 1.0f);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	sprites.push_back(newSprite);
	result = eae6320::Graphics::Sprite::Factory(newSprite, -0.5f, 0.5f, 0.25f, 0.25f);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	sprites.push_back(newSprite);

	//Texture creation
	eae6320::Graphics::cTexture::Handle newTexture;

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/texture1.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textures.push_back(newTexture);

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/texture2.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textures.push_back(newTexture);

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/texture3.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textures.push_back(newTexture);

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/texture4.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textures.push_back(newTexture);


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

	//Destroy the textures
	for (size_t i = 0; i < textures.size(); i++)
	{
		eae6320::Graphics::cTexture::s_manager.Release(textures[i]);
	}

	return Results::Success;
}
