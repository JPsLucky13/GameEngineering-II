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

	//counter += i_elapsedSecondCount_sinceLastUpdate;

	/*if (counter > timeToSwitchTexture)
	{
		std::swap(textures[0], textures[1]);
		counter = 0.0f;

	}*/

}

void eae6320::cExampleGame::UpdateSimulationBasedOnInput()
{

	isAKeyPressedForBreakOutPaddleMesh = false;

	//Check if the Paddle is within the screen space
	if (breakOutPaddleMeshRigidBody.position.x > spacePaddleMaxScreenWidth)
	{
		breakOutPaddleMeshRigidBody.position.x = spacePaddleMaxScreenWidth;
	}
	if (breakOutPaddleMeshRigidBody.position.x < -spacePaddleMaxScreenWidth)
	{
		breakOutPaddleMeshRigidBody.position.x = -spacePaddleMaxScreenWidth;
	}


	//Paddle Movement right
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		isAKeyPressedForBreakOutPaddleMesh = true;
		breakOutPaddleMeshRigidBody.velocity.x = spacePaddleSpeed;
	}

	//Paddle Movement left
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		isAKeyPressedForBreakOutPaddleMesh = true;
		breakOutPaddleMeshRigidBody.velocity.x = -spacePaddleSpeed;
	}


	//Check if the Ball hit the Space Paddle
	if (eae6320::Math::Distance(breakOutBallMeshRigidBody.position,breakOutPaddleMeshRigidBody.position) <= 0.7f)
	{
		//The ball is on top
		if (breakOutPaddleMeshRigidBody.position.y < breakOutBallMeshRigidBody.position.y)
		{
			ballSpeedX = -ballSpeedX;
			ballSpeedY = -ballSpeedY;
		}
		//The ball is on the right
		if (breakOutPaddleMeshRigidBody.position.x < breakOutBallMeshRigidBody.position.x)
		{
			ballSpeedX = -ballSpeedX;
		}
		//The ball is on the left
		if (breakOutPaddleMeshRigidBody.position.x > breakOutBallMeshRigidBody.position.x)
		{
			ballSpeedX = -ballSpeedX;
		}
	}


	//Reset the ball position if it goes below the screen
	if (breakOutBallMeshRigidBody.position.y < -ballMaxScreenWidth)
	{
		ballReleased = false;
		breakOutBallMeshRigidBody.velocity.x = 0.0f;
		breakOutBallMeshRigidBody.velocity.y = 0.0f;
		ballSpeedX = 3.0f;
		ballSpeedY = 3.0f;
		breakOutBallMeshRigidBody.position.y = breakOutPaddleMeshRigidBody.position.y + 0.5f;
	}
	
	//Check if the Ball is within the screen space and flip velocity
	if (breakOutBallMeshRigidBody.position.x > ballMaxScreenWidth || breakOutBallMeshRigidBody.position.x < -ballMaxScreenWidth)
	{
		ballSpeedX = -ballSpeedX;
	}
	if (breakOutBallMeshRigidBody.position.y > ballMaxScreenWidth)
	{
		ballSpeedY = -ballSpeedY;
	}
	

	//Ball Release
	if (!ballReleased)
	{
		breakOutBallMeshRigidBody.position.x = breakOutPaddleMeshRigidBody.position.x;
	}
	else
	{
		breakOutBallMeshRigidBody.velocity.x = ballSpeedX;
		breakOutBallMeshRigidBody.velocity.y = ballSpeedY;
	}





	if(!isAKeyPressedForBreakOutPaddleMesh)
	{
		//Stop mesh movement
		breakOutPaddleMeshRigidBody.velocity.x = 0.0f;
		breakOutPaddleMeshRigidBody.velocity.y = 0.0f;
	}
		
	//Press Space Bar to Release the Ball
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		ballReleased = true;
	}




}

void eae6320::cExampleGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	breakOutPaddleMeshRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
	breakOutBallMeshRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
	gameCamera->m_cameraRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
}



void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{

	//Render the sprite pairs
	/*for (size_t i = 0; i < sprites.size(); i++)
	{
		eae6320::Graphics::RenderSpriteWithEffectAndTexture(sprites[i],effects[i], textures[i]);
	}*/

	breakOutPaddleMeshRigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
	breakOutBallMeshRigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
	gameCamera->m_cameraRigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);

	//Submit the camera
	eae6320::Graphics::SubmitCamera(gameCamera);

	//Render the mesh with its effect
	
	//eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(meshes[0], effects[2], textures[2],meshRigidBody.position);
	//eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(meshes[2], effects[2], textures[4],secondStaticMeshPosition);
	
	//Render the Paddle
	eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(std::get<0>(meshes[0]), std::get<1>(meshes[0]), std::get<2>(meshes[0]), breakOutPaddleMeshRigidBody.position);

	//Render the Ball
	eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(std::get<0>(meshes[1]), std::get<1>(meshes[1]), std::get<2>(meshes[1]), breakOutBallMeshRigidBody.position);

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

	//The BreakOut paddle
	breakOutPaddleMeshRigidBody.position.x = 0.0f;
	breakOutPaddleMeshRigidBody.position.y = -3.8f;
	breakOutPaddleMeshRigidBody.position.z = 0.0f;

	//The BreakOut ball
	breakOutBallMeshRigidBody.position.x = breakOutPaddleMeshRigidBody.position.x;
	breakOutBallMeshRigidBody.position.y = breakOutPaddleMeshRigidBody.position.y + 0.5f;
	breakOutBallMeshRigidBody.position.z = breakOutPaddleMeshRigidBody.position.z;


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

	//Call to effect for rendering meshes
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

	//Paddle
	eae6320::Graphics::cTexture::Handle newTexture;

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/SpacePaddle.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	//Ball

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/Ball.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));


	//Mesh creation

	//Paddle
	eae6320::Graphics::Mesh::Handle newMesh;

	result = eae6320::Graphics::Mesh::s_manager.Load("data/Meshes/SpacePaddle.fbx", newMesh);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshHandles.push_back(newMesh);
	meshes.push_back(std::make_tuple(eae6320::Graphics::Mesh::s_manager.Get(newMesh), effects[1], textures[0], breakOutPaddleMeshRigidBody.position));

	result = eae6320::Graphics::Mesh::s_manager.Load("data/Meshes/Ball.fbx", newMesh);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshHandles.push_back(newMesh);
	meshes.push_back(std::make_tuple(eae6320::Graphics::Mesh::s_manager.Get(newMesh), effects[1], textures[1], breakOutBallMeshRigidBody.position));

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
