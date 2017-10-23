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

			//std::swap(textures[0], textures[1]);
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
		std::swap(textures[0], textures[1]);
		counter = 0.0f;

	}

}


void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{

	//Render the sprite pairs
	for (size_t i = 0; i < sprites.size(); i++)
	{
		eae6320::Graphics::RenderSpriteWithEffectAndTexture(sprites[i],effects[i], textures[i]);
	}

	//Render the mesh with its effect
	eae6320::Graphics::RenderMeshWithEffectAtPosition(meshes[0], effects[2], 0.0f, 0.0f);
	
	//eae6320::Graphics::RenderMeshWithEffectAtPosition(meshes[1], effects[2], -1.0f, -0.5f);

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
	
	auto result = eae6320::Graphics::Effect::Factory(newEffect, "sprite", "sprite2", eae6320::Graphics::RenderStates::AlphaTransparency);
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

	result = eae6320::Graphics::Effect::Factory(newEffect, "mesh", "mesh", eae6320::Graphics::RenderStates::AlphaTransparency);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}

	effects.push_back(newEffect);



	//Call to factory function for sprites
	result = eae6320::Graphics::Sprite::Factory(newSprite, -0.75f, 0.75f, 0.5f, 0.5f);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	sprites.push_back(newSprite);

	//Texture creation
	eae6320::Graphics::cTexture::Handle newTexture;

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/texture4.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/texture5.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	//Mesh creation
	eae6320::Graphics::Mesh * newMesh;

	//Vertex data
	eae6320::Graphics::VertexFormats::sMesh vertexData[5];

	vertexData[0].x = 0.0f;
	vertexData[0].y = 0.0f;
	vertexData[0].r = 255;
	vertexData[0].g = 0;
	vertexData[0].b = 0;
	vertexData[0].a = 255;

	vertexData[1].x = 0.0f;
	vertexData[1].y = 0.5f;
	vertexData[1].r = 0;
	vertexData[1].g = 255;
	vertexData[1].b = 0;
	vertexData[1].a = 255;

	vertexData[2].x = 0.5f;
	vertexData[2].y = 0.0f;
	vertexData[2].r = 0;
	vertexData[2].g = 0;
	vertexData[2].b = 255;
	vertexData[2].a = 255;

	vertexData[3].x = 0.5f;
	vertexData[3].y = 0.5f;
	vertexData[3].r = 255;
	vertexData[3].g = 255;
	vertexData[3].b = 0;
	vertexData[3].a = 255;

	vertexData[4].x = 0.25f;
	vertexData[4].y = 0.75f;
	vertexData[4].r = 0;
	vertexData[4].g = 255;
	vertexData[4].b = 255;
	vertexData[4].a = 255;

	//Index data
	uint16_t indexData[9] = { 0 ,1 ,2, 
							  1, 3, 2,
							  1, 4, 3};


	/*result = eae6320::Graphics::Mesh::Factory(newMesh,5,vertexData,9,indexData);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshes.push_back(newMesh);*/

	//Vertex data for the second mesh
	eae6320::Graphics::VertexFormats::sMesh vertexData2[3];

	vertexData2[0].x = -0.5f;
	vertexData2[0].y = -0.5f;
	vertexData2[0].r = 255;
	vertexData2[0].g = 0;
	vertexData2[0].b = 0;
	vertexData2[0].a = 255;

	vertexData2[1].x = 0.5f;
	vertexData2[1].y = -0.5f;
	vertexData2[1].r = 255;
	vertexData2[1].g = 0;
	vertexData2[1].b = 0;
	vertexData2[1].a = 255;

	vertexData2[2].x = 0.0f;
	vertexData2[2].y = 0.0f;
	vertexData2[2].r = 255;
	vertexData2[2].g = 0;
	vertexData2[2].b = 0;
	vertexData2[2].a = 255;

	//Index data
	uint16_t indexData2[3] = { 0 ,1 ,2 };

	result = eae6320::Graphics::Mesh::Factory(newMesh, 3, vertexData, 3, indexData2);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshes.push_back(newMesh);


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
		eae6320::Graphics::cTexture::s_manager.Release(textureHandles[i]);
	}

	//Destroy the meshes
	for (size_t i = 0; i < meshes.size(); i++)
	{
		eae6320::Graphics::Mesh::Delete(meshes[i]);
	}

	return Results::Success;
}
