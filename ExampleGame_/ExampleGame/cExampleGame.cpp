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
	//Camera Movement
	if (UserInput::IsKeyPressed('D'))
	{
		gameCamera->m_cameraRigidBody.velocity.x = 1.0f;
	}
	else if (UserInput::IsKeyPressed('A'))
	{
		gameCamera->m_cameraRigidBody.velocity.x = -1.0f;
	}
	else if (UserInput::IsKeyPressed('E'))
	{
		gameCamera->m_cameraRigidBody.velocity.z = -1.0f;
	}
	else if (UserInput::IsKeyPressed('Q'))
	{
		gameCamera->m_cameraRigidBody.velocity.z = 1.0f;
	}
	else if (UserInput::IsKeyPressed('W'))
	{
		gameCamera->m_cameraRigidBody.velocity.y = 1.0f;
	}
	else if (UserInput::IsKeyPressed('S'))
	{
		gameCamera->m_cameraRigidBody.velocity.y = -1.0f;
	}

	//Camera Rotation
	else if (UserInput::IsKeyPressed('T'))
	{
		gameCamera->m_cameraRigidBody.angularSpeed = 0.1f;
		gameCamera->m_cameraRigidBody.angularVelocity_axis_local = Math::sVector(0.0f, 1.0f, 0.0f);
	}
	else if (UserInput::IsKeyPressed('G'))
	{
		gameCamera->m_cameraRigidBody.angularSpeed = -0.1f;
		gameCamera->m_cameraRigidBody.angularVelocity_axis_local = Math::sVector(0.0f, 1.0f, 0.0f);
	}
	else if (UserInput::IsKeyPressed('Y'))
	{
		gameCamera->m_cameraRigidBody.angularSpeed = 0.1f;
		gameCamera->m_cameraRigidBody.angularVelocity_axis_local = Math::sVector(1.0f, 0.0f, 0.0f);
	}
	else if (UserInput::IsKeyPressed('H'))
	{
		gameCamera->m_cameraRigidBody.angularSpeed = -0.1f;
		gameCamera->m_cameraRigidBody.angularVelocity_axis_local = Math::sVector(1.0f, 0.0f, 0.0f);
	}

	else
	{
		gameCamera->m_cameraRigidBody.velocity.x = 0.0f;
		gameCamera->m_cameraRigidBody.velocity.y = 0.0f;
		gameCamera->m_cameraRigidBody.velocity.z = 0.0f;
		gameCamera->m_cameraRigidBody.angularSpeed = 0.0f;

	}

	//Mesh Movement
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		meshRigidBody.velocity.x = 1.0f;
	}


	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		meshRigidBody.velocity.x = -1.0f;
	}

	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		meshRigidBody.velocity.y = 1.0f;
	}

	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
	{
		meshRigidBody.velocity.y = -1.0f;
	}
	else
	{
		meshRigidBody.velocity.x = 0.0f;
		meshRigidBody.velocity.y = 0.0f;
	
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

	//Render the mesh with its effect
	
	eae6320::Graphics::RenderMeshWithEffectAtPosition(meshes[1], effects[2], floorPosition);
	eae6320::Graphics::RenderMeshWithEffectAtPosition(meshes[0], effects[2], meshRigidBody.position);

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
	eae6320::Graphics::Mesh * newMesh;

	//Vertex data
	eae6320::Graphics::VertexFormats::sMesh vertexData[8];

	//Front face
	{
		vertexData[0].x = -1.0f;
		vertexData[0].y = -1.0f;
		vertexData[0].z = 1.0f;
		vertexData[0].r = 255;
		vertexData[0].g = 0;
		vertexData[0].b = 0;
		vertexData[0].a = 255;

		vertexData[1].x = -1.0f;
		vertexData[1].y = 1.0f;
		vertexData[1].z = 1.0f;
		vertexData[1].r = 0;
		vertexData[1].g = 255;
		vertexData[1].b = 0;
		vertexData[1].a = 255;

		vertexData[2].x = 1.0f;
		vertexData[2].y = 1.0f;
		vertexData[2].z = 1.0f;
		vertexData[2].r = 0;
		vertexData[2].g = 0;
		vertexData[2].b = 255;
		vertexData[2].a = 255;

		vertexData[3].x = 1.0f;
		vertexData[3].y = -1.0f;
		vertexData[3].z = 1.0f;
		vertexData[3].r = 255;
		vertexData[3].g = 255;
		vertexData[3].b = 0;
		vertexData[3].a = 255;
	}

	//Back face
	{
		vertexData[4].x = -1.0f;
		vertexData[4].y = -1.0f;
		vertexData[4].z = -1.0f;
		vertexData[4].r = 255;
		vertexData[4].g = 0;
		vertexData[4].b = 0;
		vertexData[4].a = 255;

		vertexData[5].x = -1.0f;
		vertexData[5].y = 1.0f;
		vertexData[5].z = -1.0f;
		vertexData[5].r = 0;
		vertexData[5].g = 255;
		vertexData[5].b = 0;
		vertexData[5].a = 255;

		vertexData[6].x = 1.0f;
		vertexData[6].y = 1.0f;
		vertexData[6].z = -1.0f;
		vertexData[6].r = 0;
		vertexData[6].g = 0;
		vertexData[6].b = 255;
		vertexData[6].a = 255;

		vertexData[7].x = 1.0f;
		vertexData[7].y = -1.0f;
		vertexData[7].z = -1.0f;
		vertexData[7].r = 255;
		vertexData[7].g = 255;
		vertexData[7].b = 0;
		vertexData[7].a = 255;
	}

	//Index data
	uint16_t indexData[36] = { 0 ,1 ,2, 0, 2, 3,
							   2, 1, 5, 2, 5, 6,
							   3, 2, 6, 3, 6, 7,
							   4, 5, 1, 4, 1, 0,
							   4, 0, 3, 4, 3, 7,
							   4, 7, 6, 4, 6, 5};


	result = eae6320::Graphics::Mesh::Factory(newMesh,8,vertexData,36,indexData);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshes.push_back(newMesh);

	//Vertex data for the second mesh
	eae6320::Graphics::VertexFormats::sMesh vertexData2[4];

	vertexData2[0].x = -2.0f;
	vertexData2[0].y = -0.5f;
	vertexData2[0].z = 4.0f;
	vertexData2[0].r = 127;
	vertexData2[0].g = 127;
	vertexData2[0].b = 127;
	vertexData2[0].a = 255;

	vertexData2[1].x = -2.0f;
	vertexData2[1].y = 0.5f;
	vertexData2[1].z = -4.0f;
	vertexData2[1].r = 0;
	vertexData2[1].g = 0;
	vertexData2[1].b = 0;
	vertexData2[1].a = 255;

	vertexData2[2].x = 2.0f;
	vertexData2[2].y = 0.5f;
	vertexData2[2].z = -4.0f;
	vertexData2[2].r = 255;
	vertexData2[2].g = 255;
	vertexData2[2].b = 255;
	vertexData2[2].a = 255;

	vertexData2[3].x = 2.0f;
	vertexData2[3].y = -0.5f;
	vertexData2[3].z = 4.0f;
	vertexData2[3].r = 127;
	vertexData2[3].g = 127;
	vertexData2[3].b = 127;
	vertexData2[3].a = 255;

	////Index data
	uint16_t indexData2[6] = { 0 ,1 ,2, 0, 2, 3 };

	result = eae6320::Graphics::Mesh::Factory(newMesh, 4, vertexData2, 6, indexData2);
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

	//Delete the camera
	delete gameCamera;
	gameCamera = nullptr;

	return Results::Success;
}
