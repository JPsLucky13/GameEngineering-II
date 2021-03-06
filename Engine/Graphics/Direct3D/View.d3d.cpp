#include "../View.h"
#include "Includes.h"
#include "../sContext.h"


#include <Engine\Asserts\Asserts.h>

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::View::InitializeViews(const sInitializationParameters& i_initializationParameters)
{
	auto result = eae6320::Results::Success;

	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11Texture2D* depthBuffer = nullptr;

	auto& g_context = eae6320::Graphics::sContext::g_context;
	auto* const direct3dDevice = g_context.direct3dDevice;
	EAE6320_ASSERT(direct3dDevice);
	auto* const direct3dImmediateContext = g_context.direct3dImmediateContext;
	EAE6320_ASSERT(direct3dImmediateContext);

	// Create a "render target view" of the back buffer
	// (the back buffer was already created by the call to D3D11CreateDeviceAndSwapChain(),
	// but a "view" of it is required to use as a "render target",
	// meaning a texture that the GPU can render to)
	{
		// Get the back buffer from the swap chain
		{
			constexpr unsigned int bufferIndex = 0;	// This must be 0 since the swap chain is discarded
			const auto d3dResult = g_context.swapChain->GetBuffer(bufferIndex, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
			if (FAILED(d3dResult))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Couldn't get the back buffer from the swap chain (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to get the back buffer from the swap chain (HRESULT %#010x)", d3dResult);
				goto OnExit;
			}
		}
		// Create the view
		{
			constexpr D3D11_RENDER_TARGET_VIEW_DESC* const accessAllSubResources = nullptr;
			const auto d3dResult = direct3dDevice->CreateRenderTargetView(backBuffer, accessAllSubResources, &m_renderTargetView);
			if (FAILED(d3dResult))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Couldn't create render target view (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to create the render target view (HRESULT %#010x)", d3dResult);
				goto OnExit;
			}
		}
	}
	// Create a depth/stencil buffer and a view of it
	{
		// Unlike the back buffer no depth/stencil buffer exists until and unless it is explicitly created
		{
			D3D11_TEXTURE2D_DESC textureDescription{};
			{
				textureDescription.Width = i_initializationParameters.resolutionWidth;
				textureDescription.Height = i_initializationParameters.resolutionHeight;
				textureDescription.MipLevels = 1;	// A depth buffer has no MIP maps
				textureDescription.ArraySize = 1;
				textureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 24 bits for depth and 8 bits for stencil
				{
					DXGI_SAMPLE_DESC& sampleDescription = textureDescription.SampleDesc;

					sampleDescription.Count = 1;	// No multisampling
					sampleDescription.Quality = 0;	// Doesn't matter when Count is 1
				}
				textureDescription.Usage = D3D11_USAGE_DEFAULT;	// Allows the GPU to write to it
				textureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				textureDescription.CPUAccessFlags = 0;	// CPU doesn't need access
				textureDescription.MiscFlags = 0;
			}
			// The GPU renders to the depth/stencil buffer and so there is no initial data
			// (like there would be with a traditional texture loaded from disk)
			constexpr D3D11_SUBRESOURCE_DATA* const noInitialData = nullptr;
			const auto d3dResult = direct3dDevice->CreateTexture2D(&textureDescription, noInitialData, &depthBuffer);
			if (FAILED(d3dResult))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Couldn't create depth buffer (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to create the depth buffer resource (HRESULT %#010x)", d3dResult);
				goto OnExit;
			}
		}
		// Create the view
		{
			constexpr D3D11_DEPTH_STENCIL_VIEW_DESC* const noSubResources = nullptr;
			const auto d3dResult = direct3dDevice->CreateDepthStencilView(depthBuffer, noSubResources, &m_depthStencilView);
			if (FAILED(d3dResult))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Couldn't create depth stencil view (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to create the depth stencil view (HRESULT %#010x)", d3dResult);
				goto OnExit;
			}
		}
	}

	// Bind the views
	{
		constexpr unsigned int renderTargetCount = 1;
		direct3dImmediateContext->OMSetRenderTargets(renderTargetCount, &m_renderTargetView, m_depthStencilView);
	}
	// Specify that the entire render target should be visible
	{
		D3D11_VIEWPORT viewPort{};
		{
			viewPort.TopLeftX = viewPort.TopLeftY = 0.0f;
			viewPort.Width = static_cast<float>(i_initializationParameters.resolutionWidth);
			viewPort.Height = static_cast<float>(i_initializationParameters.resolutionHeight);
			viewPort.MinDepth = 0.0f;
			viewPort.MaxDepth = 1.0f;
		}
		constexpr unsigned int viewPortCount = 1;
		direct3dImmediateContext->RSSetViewports(viewPortCount, &viewPort);
	}

OnExit:

	// Regardless of success or failure the two texture resources should be released
	// (if successful the views will hold internal references to the resources)
	if (backBuffer)
	{
		backBuffer->Release();
		backBuffer = nullptr;
	}
	if (depthBuffer)
	{
		depthBuffer->Release();
		depthBuffer = nullptr;
	}

	return result;
}

void eae6320::Graphics::View::CleanUp()
{
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}
}

void eae6320::Graphics::View::ClearColor(float red, float green, float blue, float alpha)
{

	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		EAE6320_ASSERT(m_renderTargetView);

		// Black is usually used
		float clearColor[4] = { red, green, blue, alpha };
		auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
		EAE6320_ASSERT(direct3dImmediateContext);
		direct3dImmediateContext->ClearRenderTargetView(m_renderTargetView, clearColor);
	}
}

void eae6320::Graphics::View::ClearDepthBuffer(float i_depth)
{
	EAE6320_ASSERT(m_depthStencilView);
	constexpr uint8_t stencilValue = 0; // Arbitrary if stencil isn't used
	auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
	EAE6320_ASSERT(direct3dImmediateContext);
	direct3dImmediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, i_depth, stencilValue);
}

void eae6320::Graphics::View::ViewSwapBuffers()
{
	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer")
	{
		auto* const swapChain = sContext::g_context.swapChain;
		EAE6320_ASSERT(swapChain);
		constexpr unsigned int swapImmediately = 0;
		constexpr unsigned int presentNextFrame = 0;
		const auto result = swapChain->Present(swapImmediately, presentNextFrame);
		EAE6320_ASSERT(SUCCEEDED(result));
	}

	// Once everything has been drawn the data that was submitted for this frame
	// should be cleaned up and cleared.
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	{
		// (At this point in the class there isn't anything that needs to be cleaned up)
	}

}