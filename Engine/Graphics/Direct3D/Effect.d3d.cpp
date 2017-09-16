#include "../Effect.h"
#include "Includes.h"
#include "../sContext.h"


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
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dImmediateContext);
			direct3dImmediateContext->VSSetShader(shader->m_shaderObject.vertex, noInterfaces, interfaceCount);
		}
		// Fragment shader
		{
			EAE6320_ASSERT(m_fragmentShader);
			auto* const shader = cShader::s_manager.Get(m_fragmentShader);
			EAE6320_ASSERT(shader && shader->m_shaderObject.fragment);
			auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
			EAE6320_ASSERT(direct3dImmediateContext);
			direct3dImmediateContext->PSSetShader(shader->m_shaderObject.fragment, noInterfaces, interfaceCount);
		}
	}
	BindRenderState();


}

eae6320::cResult eae6320::Graphics::Effect::CreateProgram(eae6320::cResult & result) {
	return result;
}

eae6320::cResult eae6320::Graphics::Effect::CheckProgramID(eae6320::cResult & result)
{
	return result;

}