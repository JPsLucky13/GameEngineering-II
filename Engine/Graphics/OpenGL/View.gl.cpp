#include "../View.h"
#include "Includes.h"

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::Graphics::View::InitializeViews(const sInitializationParameters& i_initializationParameters)
{
	auto result = eae6320::Results::Success;
	
	return result;
}

void eae6320::Graphics::View::CleanUp()
{
	
}

void eae6320::Graphics::View::ClearColor(float red, float green, float blue, float alpha)
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		{
			glClearColor(red, green, blue, alpha);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
			glClear(clearColor);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
}

void eae6320::Graphics::View::ViewSwapBuffers()
{
	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer")
	{
		const auto deviceContext = sContext::g_context.deviceContext;
		EAE6320_ASSERT(deviceContext != NULL);

		const auto glResult = SwapBuffers(deviceContext);
		EAE6320_ASSERT(glResult != FALSE);
	}

	// Once everything has been drawn the data that was submitted for this frame
	// should be cleaned up and cleared.
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	{
		// (At this point in the class there isn't anything that needs to be cleaned up)
	}
}