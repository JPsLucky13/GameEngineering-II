#include "Effect.h"
#include "sContext.h"
#include <string.h>

#include <Engine\Asserts\Asserts.h>

// Implementation
//===============

eae6320::cResult eae6320::Graphics::Effect::Factory(eae6320::Graphics::Effect* &o_effect, char * vertexShaderName, char * fragmentShaderName, uint8_t defaultRenderState)
{
	auto result = Results::Success;

	Effect* newEffect = nullptr;

	// Allocate a new effect
	{
		newEffect = new Effect();
		if (!newEffect)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the effect");
			Logging::OutputError("Failed to allocate memory for the effect");
			goto OnExit;
		}
	}

	// Initialize the platform-specific graphics API effect object
	if (!(result = newEffect->Initialize(vertexShaderName, fragmentShaderName, defaultRenderState)))
	{
		EAE6320_ASSERTF(false, "Initialization of new effect failed");
		goto OnExit;
	}

OnExit:

	if (result)
	{
		EAE6320_ASSERT(newEffect);
		o_effect = newEffect;
	}
	else
	{
		if (newEffect)
		{
			newEffect->DecrementReferenceCount();
			newEffect = nullptr;
		}
		o_effect = nullptr;
	}

	return result;

}

void eae6320::Graphics::Effect::Delete(eae6320::Graphics::Effect* &o_effect)
{
	o_effect->DecrementReferenceCount();
	o_effect = nullptr;
}

// Initialization / Clean Up
//--------------------------
eae6320::cResult eae6320::Graphics::Effect::Initialize(char * vertexShaderName, char * fragmentShaderName, uint8_t defaultRenderState)
{
	auto result = eae6320::Results::Success;

	char path[256];
	strcpy(path, "data/Shaders/Vertex/");
	strcat(path, vertexShaderName);
	strcat(path, ".shd");
	puts(path);

	if (!(result = eae6320::Graphics::cShader::s_manager.Load(path,
		m_vertexShader, eae6320::Graphics::ShaderTypes::Vertex)))
	{
		EAE6320_ASSERT(false);
		goto OnExit;
	}

	char path2[256];
	strcpy(path2, "data/Shaders/Fragment/");
	strcat(path2, fragmentShaderName);
	strcat(path2, ".shd");
	puts(path2);
	if (!(result = eae6320::Graphics::cShader::s_manager.Load(path2,
		m_fragmentShader, eae6320::Graphics::ShaderTypes::Fragment)))
	{
		EAE6320_ASSERT(false);
		goto OnExit;
	}
	{
		if (!(result = m_renderState.Initialize(defaultRenderState)))
		{
			EAE6320_ASSERT(false);
			goto OnExit;
		}
	}

	result = CreateProgram(result);


OnExit:
	return result;

}

void eae6320::Graphics::Effect::BindRenderState()
{
	m_renderState.Bind();
}

eae6320::Graphics::cRenderState eae6320::Graphics::Effect::GetRenderState()
{
	return m_renderState;
}


eae6320::cResult eae6320::Graphics::Effect::CleanUp()
{
	auto result = Results::Success;

	result = CheckProgramID(result);


	if (m_vertexShader)
	{
		const auto localResult = cShader::s_manager.Release(m_vertexShader);
		if (!localResult)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = localResult;
			}
		}
	}
	if (m_fragmentShader)
	{
		const auto localResult = cShader::s_manager.Release(m_fragmentShader);
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
		const auto localResult = m_renderState.CleanUp();
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

eae6320::Graphics::Effect::Effect()
{

}

eae6320::Graphics::Effect::~Effect()
{
	CleanUp();
}
