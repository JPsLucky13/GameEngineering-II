#include "Effect.h"
#include "sContext.h"
#include <string.h>

#include <Engine\Asserts\Asserts.h>

// Implementation
//===============


// Initialization / Clean Up
//--------------------------
eae6320::cResult eae6320::Graphics::Effect::Initialize(char * vertexShaderName, char * fragmentShaderName)
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
		constexpr uint8_t defaultRenderState = 0;
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

void eae6320::Graphics::Effect::CleanUp(eae6320::cResult & result)
{

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

}

