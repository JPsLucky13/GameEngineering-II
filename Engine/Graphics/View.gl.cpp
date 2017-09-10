#include "View.h"
#include "..//Graphics//OpenGL/Includes.h"

// Implementation
//===============

void eae6320::Graphics::View::GetContext()
{

}

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