#include "Effect.h"
#include "..//Graphics//Direct3D/Includes.h"
#include "sContext.h"


#include <Engine\Asserts\Asserts.h>

// Implementation
//===============

// Initialization / Clean Up
//--------------------------

void eae6320::Graphics::Effect::Bind()
{
	{
		ID3D11ClassInstance* const* noInterfaces = nullptr;
		constexpr unsigned int interfaceCount = 0;
		// Vertex shader
		{
			EAE6320_ASSERT(m_vertexShader);
			auto* const shader = cShader::s_manager.Get(m_vertexShader);
			EAE6320_ASSERT(shader && shader->m_shaderObject.vertex);
			m_direct3dContext->VSSetShader(shader->m_shaderObject.vertex, noInterfaces, interfaceCount);
		}
		// Fragment shader
		{
			EAE6320_ASSERT(m_fragmentShader);
			auto* const shader = cShader::s_manager.Get(m_fragmentShader);
			EAE6320_ASSERT(shader && shader->m_shaderObject.fragment);
			m_direct3dContext->PSSetShader(shader->m_shaderObject.fragment, noInterfaces, interfaceCount);
		}
	}
	m_renderState.Bind();


}

eae6320::cResult eae6320::Graphics::Effect::CreateProgram(eae6320::cResult & result) {
	return result;
}

eae6320::cResult eae6320::Graphics::Effect::CheckProgramID(eae6320::cResult & result)
{
	return result;

}