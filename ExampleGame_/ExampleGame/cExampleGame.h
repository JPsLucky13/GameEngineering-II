/*
	This class is your specific game
*/

#ifndef EAE6320_CEXAMPLEGAME_H
#define EAE6320_CEXAMPLEGAME_H

// Include Files
//==============

#include <Engine/Application/cbApplication.h>
#include <Engine/Results/Results.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/cMatrix_transformation.h>


#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif

// Class Declaration
//==================

namespace eae6320
{
	class cExampleGame : public Application::cbApplication
	{
		// Inherited Implementation
		//=========================


	private:

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		virtual const char* GetMainWindowName() const override
		{
			return "Jean-Paul's EAE6320 Example Game"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
				;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		virtual const char* GetMainWindowClassName() const override { return "Jean-Paul's EAE6320 Example Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		virtual const WORD* GetLargeIconId() const override { static constexpr WORD iconId_large = IDI_EAEALIEN; return &iconId_large; }
		virtual const WORD* GetSmallIconId() const override { static constexpr WORD iconId_small = IDI_EAEALIEN; return &iconId_small; }

		// KeyPressState
		//-------------
		bool isKeyPressedAlready;
		
		// KeyPressForBreakOutPaddleMesh
		//-------------
		bool isAKeyPressedForBreakOutPaddleMesh;

		// KeyPressForCamera
		//-------------
		bool isAKeyPressedForCamera;

		// Elapsed Time
		//-------------
		float counter = 0.0f;
		float timeToSwitchTexture = 1.0f;

		// Space Paddle Stats
		//-------------
		float spacePaddleSpeed = 5.0f;
		float spacePaddleMaxScreenWidth = 3.5f;

		// Ball Stats
		//-------------
		float ballSpeedX = 3.0f;
		float ballSpeedY = 3.0f;
		float ballMaxScreenWidth = 3.8f;
		bool ballReleased = false;

		// Back ground flash
		//-------------
		float ScreenFeedbackR = 0.0f;
		float ScreenFeedbackG = 0.0f;
		float ScreenFeedbackB = 0.0f;
		float feedbackCounter = 0.0f;
		float timeToBlack = 0.25f;
		float maxFeedbackCounterValue = 1.0f;
		
		float endStateCounter = 0.0f;
		float timeToErase = 0.75f;
		float maxEndStateCounterValue = 1.0f;
		size_t endStateMessage = 5; //Value has to be 3,4 or 5 
		

		// Effects
		//-------------
		std::vector<eae6320::Graphics::Effect *> effects;

		// Sprites
		//--------------
		std::vector<eae6320::Graphics::Sprite *> sprites;

		// Textures
		//--------------
		std::vector<eae6320::Graphics::cTexture::Handle> textureHandles;
		std::vector<eae6320::Graphics::cTexture *> textures;

		// Meshes
		//--------------
		std::vector<eae6320::Graphics::Mesh::Handle> meshHandles;
		std::vector<std::tuple<eae6320::Graphics::Mesh *, eae6320::Graphics::Effect *, eae6320::Graphics::cTexture *,Math::sVector>> meshes;

		// Camera
		//--------------
		eae6320::Graphics::Camera * gameCamera;
		eae6320::Physics::sRigidBodyState cameraRigidBody;

		// BeakOut Paddle Mesh sRigidbody
		//--------------
		eae6320::Physics::sRigidBodyState breakOutPaddleMeshRigidBody;

		// BeakOut Ball Mesh sRigidbody
		//--------------
		eae6320::Physics::sRigidBodyState breakOutBallMeshRigidBody;

		// Static Mesh
		Math::sVector floorPosition;

		// Second Static Mesh
		Math::sVector secondStaticMeshPosition;

#endif

		// Run
		//----

		virtual void UpdateBasedOnInput() override;
		virtual void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;

		virtual void UpdateSimulationBasedOnInput() override;
		virtual void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;

		// Submit render data

		virtual void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate) override;

		// Initialization / Clean Up
		//--------------------------

		virtual cResult Initialize() override;
		virtual cResult CleanUp() override;

	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, System::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CEXAMPLEGAME_H
