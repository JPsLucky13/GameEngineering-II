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

void eae6320::cExampleGame::UpdateSimulationBasedOnInput()
{

	isAKeyPressedForMovableMesh = false;
	isAKeyPressedForCamera = false;

	//Camera Movement
	if (UserInput::IsKeyPressed('D'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.velocity.x = 1.0f;
	}
	if (UserInput::IsKeyPressed('A'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.velocity.x = -1.0f;
	}
	if (UserInput::IsKeyPressed('E'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.velocity.z = -1.0f;
	}
	if (UserInput::IsKeyPressed('Q'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.velocity.z = 1.0f;
	}
	if (UserInput::IsKeyPressed('W'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.velocity.y = 1.0f;
	}
	if (UserInput::IsKeyPressed('S'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.velocity.y = -1.0f;
	}

	//Camera Rotation
	if (UserInput::IsKeyPressed('T'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.angularSpeed = 0.1f;
		gameCamera->m_cameraRigidBody.angularVelocity_axis_local = Math::sVector(0.0f, 1.0f, 0.0f);
	}
	if (UserInput::IsKeyPressed('G'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.angularSpeed = -0.1f;
		gameCamera->m_cameraRigidBody.angularVelocity_axis_local = Math::sVector(0.0f, 1.0f, 0.0f);
	}
	if (UserInput::IsKeyPressed('Y'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.angularSpeed = 0.1f;
		gameCamera->m_cameraRigidBody.angularVelocity_axis_local = Math::sVector(1.0f, 0.0f, 0.0f);
	}
	if (UserInput::IsKeyPressed('H'))
	{
		isAKeyPressedForCamera = true;
		gameCamera->m_cameraRigidBody.angularSpeed = -0.1f;
		gameCamera->m_cameraRigidBody.angularVelocity_axis_local = Math::sVector(1.0f, 0.0f, 0.0f);
	}


	//Mesh Movement
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		isAKeyPressedForMovableMesh = true;
		meshRigidBody.velocity.x = 1.0f;
	}


	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		isAKeyPressedForMovableMesh = true;
		meshRigidBody.velocity.x = -1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		isAKeyPressedForMovableMesh = true;
		meshRigidBody.velocity.y = 1.0f;
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
	{
		isAKeyPressedForMovableMesh = true;
		meshRigidBody.velocity.y = -1.0f;
	}

	if(!isAKeyPressedForMovableMesh)
	{
		//Stop mesh movement
		meshRigidBody.velocity.x = 0.0f;
		meshRigidBody.velocity.y = 0.0f;
	}

	if (!isAKeyPressedForCamera)
	{
		//Stop Camera movement and rotation
		gameCamera->m_cameraRigidBody.velocity.x = 0.0f;
		gameCamera->m_cameraRigidBody.velocity.y = 0.0f;
		gameCamera->m_cameraRigidBody.velocity.z = 0.0f;
		gameCamera->m_cameraRigidBody.angularSpeed = 0.0f;
	}
		
}

void eae6320::cExampleGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	meshRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
	gameCamera->m_cameraRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
}


void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{

	//Render the sprite pairs
	for (size_t i = 0; i < sprites.size(); i++)
	{
		eae6320::Graphics::RenderSpriteWithEffectAndTexture(sprites[i],effects[i], textures[i]);
	}

	meshRigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
	gameCamera->m_cameraRigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);


	//Render the mesh with its effect
	
	eae6320::Graphics::RenderMeshWithEffectAtPosition(meshes[0], effects[2], meshRigidBody.position);
	eae6320::Graphics::RenderMeshWithEffectAtPosition(meshes[1], effects[2], floorPosition);

	//Submit the camera
	eae6320::Graphics::SubmitCamera(gameCamera);

	//User specify's the background clear color
	eae6320::Graphics::ClearColor(0.5f,0.0f,0.0f,1.0f);
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cExampleGame::Initialize()
{
	//The Game Camera
	cameraRigidBody.position.x = 0.0f;
	cameraRigidBody.position.y = 0.0f;
	cameraRigidBody.position.z = 10.0f;

	gameCamera = new eae6320::Graphics::Camera(cameraRigidBody,45.0f,1.0f,0.1f,100.0f);

	//Movable mesh initial position
	meshRigidBody.position.x = 0.0f;
	meshRigidBody.position.y = 2.0f;
	meshRigidBody.position.z = 0.0f;

	//Static mesh position
	floorPosition.x = 0.0f;
	floorPosition.y = -1.0f;
	floorPosition.z = 0.0f;

	//Effect and sprite pointers
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

	result = eae6320::Graphics::Effect::Factory(newEffect, "mesh", "mesh", eae6320::Graphics::RenderStates::DepthBuffering);
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
	eae6320::Graphics::Mesh::Handle newMesh;

	result = eae6320::Graphics::Mesh::s_manager.Load("data/Meshes/MovableMesh.fbx",newMesh);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshHandles.push_back(newMesh);
	meshes.push_back(eae6320::Graphics::Mesh::s_manager.Get(newMesh));

	result = eae6320::Graphics::Mesh::s_manager.Load("data/Meshes/StaticMesh.fbx", newMesh);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshHandles.push_back(newMesh);
	meshes.push_back(eae6320::Graphics::Mesh::s_manager.Get(newMesh));

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
		eae6320::Graphics::Mesh::s_manager.Release(meshHandles[i]);
	}

	//Delete the camera
	delete gameCamera;
	gameCamera = nullptr;

	return Results::Success;
}
