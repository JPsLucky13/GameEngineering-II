// Include Files
//==============

#include "Graphics.h"


#include "cConstantBuffer.h"
#include "ConstantBufferFormats.h"
#include "cRenderState.h"
#include "cSamplerState.h"
#include "cShader.h"
#include "sContext.h"
#include "VertexFormats.h"
#include "View.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>
#include <utility>
#include <algorithm>

// Helper Function Declarations
//=============================

namespace
{
	eae6320::cResult InitializeGeometry();
	eae6320::cResult InitializeShadingData();
}

// Static Data Initialization
//===========================

namespace
{

	// Constant buffer object
	eae6320::Graphics::cConstantBuffer s_constantBuffer_perFrame(eae6320::Graphics::ConstantBufferTypes::PerFrame);
	// Constant buffer object
	eae6320::Graphics::cConstantBuffer s_constantBuffer_perDrawCall(eae6320::Graphics::ConstantBufferTypes::PerDrawCall);
	// In our class we will only have a single sampler state
	eae6320::Graphics::cSamplerState s_samplerState;

	// Submission Data
	//----------------

	// This struct's data is populated at submission time;
	// it must cache whatever is necessary in order to render a frame
	struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sPerFrame constantData_perFrame;
		
		
		//ColorData
		float red;
		float green;
		float blue;
		float alpha;

		//Effect
		std::vector<eae6320::Graphics::Effect *> effects;

		//Sprite
		std::vector<eae6320::Graphics::Sprite *> sprites;

		//Textures
		std::vector<eae6320::Graphics::cTexture *> textures;

		eae6320::Graphics::ConstantBufferFormats::sPerDrawCall constantData_perDrawCall;

		//Meshes
		std::vector<std::tuple <eae6320::Graphics::Mesh *, eae6320::Graphics::Effect *, eae6320::Graphics::cTexture *, eae6320::Math::sVector>> meshTuples;

	};

	// In our class there will be two copies of the data required to render a frame:
	//	* One of them will be getting populated by the data currently being submitted by the application loop thread
	//	* One of them will be fully populated, 
	sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
	auto* s_dataBeingSubmittedByApplicationThread = &s_dataRequiredToRenderAFrame[0];
	auto* s_dataBeingRenderedByRenderThread = &s_dataRequiredToRenderAFrame[1];

	// The following two events work together to make sure that
	// the main/render thread and the application loop thread can work in parallel but stay in sync:
	// This event is signaled by the application loop thread when it has finished submitting render data for a frame
	// (the main/render thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenAllDataHasBeenSubmittedFromApplicationThread;
	// This event is signaled by the main/render thread when it has swapped render data pointers.
	// This means that the renderer is now working with all the submitted data it needs to render the next frame,
	// and the application loop thread can start submitting data for the following frame
	// (the application loop thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenDataForANewFrameCanBeSubmittedFromApplicationThread;

	//View Data
	//--------------
	eae6320::Graphics::View view;
}

// Interface
//==========

// Submission
//-----------

void eae6320::Graphics::SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread);
	auto& constantData_perFrame = s_dataBeingSubmittedByApplicationThread->constantData_perFrame;
	constantData_perFrame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_perFrame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;

}

eae6320::cResult eae6320::Graphics::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
{
	return Concurrency::WaitForEvent(s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
}

eae6320::cResult eae6320::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}


// Render
//-------
void eae6320::Graphics::ClearColor(float red, float green, float blue, float alpha)
{
	//Colors to clear the screen
	s_dataBeingSubmittedByApplicationThread->red = red;
	s_dataBeingSubmittedByApplicationThread->green = green;
	s_dataBeingSubmittedByApplicationThread->blue = blue;
	s_dataBeingSubmittedByApplicationThread->alpha = alpha;

	
}

void eae6320::Graphics::RenderSpriteWithEffectAndTexture(Sprite * sprite, Effect * effect, cTexture * texture)
{
		sprite->IncrementReferenceCount();
		effect->IncrementReferenceCount();
		texture->IncrementReferenceCount();

		s_dataBeingSubmittedByApplicationThread->effects.push_back(effect);
		s_dataBeingSubmittedByApplicationThread->sprites.push_back(sprite);
		s_dataBeingSubmittedByApplicationThread->textures.push_back(texture);
}

void eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(Mesh * mesh, Effect * effect,cTexture * texture, Math::sVector position)
{
	mesh->IncrementReferenceCount();
	effect->IncrementReferenceCount();
	texture->IncrementReferenceCount();

	s_dataBeingSubmittedByApplicationThread->meshTuples.push_back(std::make_tuple(mesh,effect,texture,position));
}

void eae6320::Graphics::SubmitCamera(Camera * camera)
{
	s_dataBeingSubmittedByApplicationThread->constantData_perFrame.g_transform_worldToCamera = eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform(camera->m_cameraRigidBody.orientation, camera->m_cameraRigidBody.position);
	s_dataBeingSubmittedByApplicationThread->constantData_perFrame.g_transform_cameraToProjected = eae6320::Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(Math::ConvertDegreesToRadians(camera->m_verticalFieldOfView_inRadians),camera->m_aspectRatio, camera->m_z_nearPlane,camera->m_z_farPlane);
}

bool MeshSortCondition(const  std::tuple<eae6320::Graphics::Mesh *, eae6320::Graphics::Effect *, eae6320::Graphics::cTexture *, eae6320::Math::sVector>& left, const std::tuple<eae6320::Graphics::Mesh *, eae6320::Graphics::Effect *, eae6320::Graphics::cTexture *, eae6320::Math::sVector>& right)
{
	eae6320::Math::sVector leftCameraPosition = s_dataBeingSubmittedByApplicationThread->constantData_perFrame.g_transform_worldToCamera * std::get<3>(left);
	eae6320::Math::sVector rightCameraPosition = s_dataBeingSubmittedByApplicationThread->constantData_perFrame.g_transform_worldToCamera * std::get<3>(right);

	return leftCameraPosition.z < rightCameraPosition.z;
}

void eae6320::Graphics::SortTranslucentMeshes(std::vector<std::tuple<eae6320::Graphics::Mesh *, eae6320::Graphics::Effect *, eae6320::Graphics::cTexture *, eae6320::Math::sVector>> & meshTuples)
{
	//Translucent
	std::sort(meshTuples.begin(), meshTuples.end(), MeshSortCondition);
}


void eae6320::Graphics::RenderFrame()
{
	// Wait for the application loop to submit data to be rendered
	{
		const auto result = Concurrency::WaitForEvent(s_whenAllDataHasBeenSubmittedFromApplicationThread);
		if (result)
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap(s_dataBeingSubmittedByApplicationThread, s_dataBeingRenderedByRenderThread);
			// Once the pointers have been swapped the application loop can submit new data
			const auto result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal();
			if (!result)
			{
				EAE6320_ASSERTF(false, "Couldn't signal that new graphics data can be submitted");
				Logging::OutputError("Failed to signal that new render data can be submitted");
				UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited");
				return;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Waiting for the graphics data to be submitted failed");
			Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
			UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited");
			return;
		}
	}

	EAE6320_ASSERT(s_dataBeingRenderedByRenderThread);

	// Update the per-frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
		auto& constantData_perFrame = s_dataBeingRenderedByRenderThread->constantData_perFrame;
		s_constantBuffer_perFrame.Update(&constantData_perFrame);
		
	}

	//Clear the screen with color 
	{
		// Clear the frame
		view.ClearColor(s_dataBeingRenderedByRenderThread->red, s_dataBeingRenderedByRenderThread->green,
			s_dataBeingRenderedByRenderThread->blue, s_dataBeingRenderedByRenderThread->alpha);

	}

	//Clear the depth buffer
	{
		view.ClearDepthBuffer(1.0f);
	}

	//Bind effects and draw sprites
	{

		for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->meshTuples.size(); i++)
		{
			std::get<1>(s_dataBeingRenderedByRenderThread->meshTuples[i])->Bind();
			std::get<2>(s_dataBeingRenderedByRenderThread->meshTuples[i])->Bind(0);
			auto& constantData_perDrawCall = s_dataBeingSubmittedByApplicationThread->constantData_perDrawCall;
			constantData_perDrawCall.g_transform_localToWorld = Math::cMatrix_transformation(Math::cQuaternion(), std::get<3>(s_dataBeingRenderedByRenderThread->meshTuples[i]));
			s_constantBuffer_perDrawCall.Update(&constantData_perDrawCall);
			std::get<0>(s_dataBeingRenderedByRenderThread->meshTuples[i])->Draw();
		}

		for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->sprites.size(); i++)
		{
			s_dataBeingRenderedByRenderThread->effects[i]->Bind();
			s_dataBeingRenderedByRenderThread->textures[i]->Bind(0);
			s_dataBeingRenderedByRenderThread->sprites[i]->Draw();
		}

	}

	//Swap the buffers
	view.ViewSwapBuffers();

	//Reset the arbitrary number of sprites, effects and textures
	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->effects.size(); i++)
	{
		s_dataBeingRenderedByRenderThread->effects[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->sprites.size(); i++)
	{
		s_dataBeingRenderedByRenderThread->sprites[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->textures.size(); i++)
	{
		s_dataBeingRenderedByRenderThread->textures[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->meshTuples.size(); i++)
	{
		std::get<0>(s_dataBeingRenderedByRenderThread->meshTuples[i])->DecrementReferenceCount();
		std::get<1>(s_dataBeingRenderedByRenderThread->meshTuples[i])->DecrementReferenceCount();
		std::get<2>(s_dataBeingRenderedByRenderThread->meshTuples[i])->DecrementReferenceCount();
	}
	//Clear the arbitrary number of sprites, effects, textures and meshes
	s_dataBeingRenderedByRenderThread->effects.clear();
	s_dataBeingRenderedByRenderThread->sprites.clear();
	s_dataBeingRenderedByRenderThread->textures.clear();
	s_dataBeingRenderedByRenderThread->meshTuples.clear();
}


// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	// Initialize the platform-specific context
	if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
	{
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	// Initialize the asset managers
	{
		if (!(result = cShader::s_manager.Initialize()))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

	// Initialize the platform-independent graphics objects
	{
		if (result = s_constantBuffer_perFrame.Initialize())
		{
			// There is only a single per-frame constant buffer that is re-used
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_perFrame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				ShaderTypes::Vertex | ShaderTypes::Fragment);
		}
		else
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		if (result = s_constantBuffer_perDrawCall.Initialize())
		{
			// There is only a single per-frame constant buffer that is re-used
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_perDrawCall.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				ShaderTypes::Vertex | ShaderTypes::Fragment);
		}
		else
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		if (result = s_samplerState.Initialize())
		{
			// There is only a single sampler state that is re-used
			// and so it can be bound at initialization time and never unbound
			s_samplerState.Bind();
		}
		else
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}
	// Initialize the events
	{
		if (!(result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
		if (!(result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}
	// Initialize the views
	{
		if (!(result = view.InitializeViews(i_initializationParameters)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}
	// Initialize the shading data
	{
		if (!(result = InitializeShadingData()))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}
	// Initialize the geometry
	{
		if (!(result = InitializeGeometry()))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

OnExit:

	return result;
}

eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = Results::Success;

	//Clean up the view object
	view.CleanUp();

	//Reset the arbitrary number of sprites and effects
	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->effects.size(); i++)
	{
		s_dataBeingRenderedByRenderThread->effects[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->sprites.size(); i++)
	{
		s_dataBeingRenderedByRenderThread->sprites[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->textures.size(); i++)
	{
		s_dataBeingRenderedByRenderThread->textures[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingRenderedByRenderThread->meshTuples.size(); i++)
	{
		std::get<0>(s_dataBeingRenderedByRenderThread->meshTuples[i])->DecrementReferenceCount();
		std::get<1>(s_dataBeingRenderedByRenderThread->meshTuples[i])->DecrementReferenceCount();
		std::get<2>(s_dataBeingRenderedByRenderThread->meshTuples[i])->DecrementReferenceCount();
	}

	//Clear the arbitrary number of sprites and effects
	s_dataBeingRenderedByRenderThread->effects.clear();
	s_dataBeingRenderedByRenderThread->sprites.clear();
	s_dataBeingRenderedByRenderThread->textures.clear();
	s_dataBeingRenderedByRenderThread->meshTuples.clear();

	//Reset the arbitrary number of sprites and effects
	for (size_t i = 0; i < s_dataBeingSubmittedByApplicationThread->effects.size(); i++)
	{
		s_dataBeingSubmittedByApplicationThread->effects[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingSubmittedByApplicationThread->sprites.size(); i++)
	{
		s_dataBeingSubmittedByApplicationThread->sprites[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingSubmittedByApplicationThread->textures.size(); i++)
	{
		s_dataBeingSubmittedByApplicationThread->textures[i]->DecrementReferenceCount();
	}
	for (size_t i = 0; i < s_dataBeingSubmittedByApplicationThread->meshTuples.size(); i++)
	{
		std::get<0>(s_dataBeingSubmittedByApplicationThread->meshTuples[i])->DecrementReferenceCount();
		std::get<1>(s_dataBeingSubmittedByApplicationThread->meshTuples[i])->DecrementReferenceCount();
		std::get<2>(s_dataBeingSubmittedByApplicationThread->meshTuples[i])->DecrementReferenceCount();
	}
	//Clear the arbitrary number of sprites and effects
	s_dataBeingSubmittedByApplicationThread->effects.clear();
	s_dataBeingSubmittedByApplicationThread->sprites.clear();
	s_dataBeingSubmittedByApplicationThread->textures.clear();
	s_dataBeingSubmittedByApplicationThread->meshTuples.clear();

	{
		const auto localResult = s_constantBuffer_perFrame.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}
	{
		const auto localResult = s_constantBuffer_perDrawCall.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}
	{
		const auto localResult = s_samplerState.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}

	{
		const auto localResult = cShader::s_manager.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}

	{
		const auto localResult = sContext::g_context.CleanUp();
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}

	return result;
}

// Helper Function Definitions
//============================

namespace
{
	eae6320::cResult InitializeGeometry()
	{
		auto result = eae6320::Results::Success;
		return result;
	}

	eae6320::cResult InitializeShadingData()
	{
		auto result = eae6320::Results::Success;
		return result;
	}
}