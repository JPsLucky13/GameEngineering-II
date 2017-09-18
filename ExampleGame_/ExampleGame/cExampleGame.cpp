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
	eae6320::Graphics::RenderSpriteWithEffect(m_sprite1,m_effect1,1);
	eae6320::Graphics::RenderSpriteWithEffect(m_sprite2,m_effect2,1);
	eae6320::Graphics::RenderSpriteWithEffect(m_sprite3,m_effect2,1);

	//User specify's the background clear color
	eae6320::Graphics::ClearColor(0.5f,0.0f,0.0f,1.0f);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cExampleGame::Initialize()
{
	//Call to factory function for effects
	eae6320::Graphics::Effect::Factory(m_effect1, "sprite", "sprite1", eae6320::Graphics::RenderStates::AlphaTransparency);
	eae6320::Graphics::Effect::Factory(m_effect2, "sprite", "sprite2", eae6320::Graphics::RenderStates::AlphaTransparency);

	//Call to factory function for sprites
	eae6320::Graphics::Sprite::Factory(m_sprite1, 0.5f, 0.5f, 0.5f, 0.5f);
	eae6320::Graphics::Sprite::Factory(m_sprite2, -0.5f, -0.5f, 1.0f, 1.0f);
	eae6320::Graphics::Sprite::Factory(m_sprite3, -0.5f, 0.5f, 0.25f, 0.25f);

	return Results::Success;
}

eae6320::cResult eae6320::cExampleGame::CleanUp()
{
	//Destroy the effects
	eae6320::Graphics::Effect::Delete(m_effect1);
	eae6320::Graphics::Effect::Delete(m_effect2);

	//Destroy the sprites
	eae6320::Graphics::Sprite::Delete(m_sprite1);
	eae6320::Graphics::Sprite::Delete(m_sprite2);
	eae6320::Graphics::Sprite::Delete(m_sprite3);

	return Results::Success;
}
