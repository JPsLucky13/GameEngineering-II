#include "Mesh.h"

#include "sContext.h"


#include <Engine\Asserts\Asserts.h>
#include <Engine/Platform/Platform.h>
#include <External/Lua/Includes.h>
#include <iostream>

// Helper Function Declarations
//=============================

namespace
{
	unsigned int s_vertexCount;
	unsigned int s_indexCount;
	eae6320::Graphics::VertexFormats::sMesh s_vertexData[10000];
	uint16_t s_indexData[10000];

	eae6320::cResult LoadTableValues(lua_State& io_luaState);

	eae6320::cResult LoadTableValues_vertices(lua_State& io_luaState);
	eae6320::cResult LoadTableValues_vertices_values(lua_State& io_luaState);

	eae6320::cResult LoadTableValues_indices(lua_State& io_luaState);
	eae6320::cResult LoadTableValues_indices_values(lua_State& io_luaState);

	eae6320::cResult LoadAsset(const char* const i_path);
}

// Interface
//==========


// Helper Function Definitions
//============================

namespace
{
	eae6320::cResult LoadTableValues(lua_State& io_luaState)
	{
		auto result = eae6320::Results::Success;

		if (!(result = LoadTableValues_vertices(io_luaState)))
		{
			return result;
		}
		if (!(result = LoadTableValues_indices(io_luaState)))
		{
			return result;
		}

		return result;
	}

	eae6320::cResult LoadTableValues_vertices(lua_State& io_luaState)
	{
		auto result = eae6320::Results::Success;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "vertices" table will be at -1:
		constexpr auto* const key = "vertices";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		// It can be hard to remember where the stack is at
		// and how many values to pop.
		// One strategy I would suggest is to always call a new function
		// When you are at a new level:
		// Right now we know that we have an original table at -2,
		// and a new one at -1,
		// and so we _know_ that we always have to pop at least _one_
		// value before leaving this function
		// (to make the original table be back to index -1).
		// If we don't do any further stack manipulation in this function
		// then it becomes easy to remember how many values to pop
		// because it will always be one.
		// This is the strategy I'll take in this example
		// (look at the "OnExit" label):
		if (lua_istable(&io_luaState, -1))
		{
			if (!(result = LoadTableValues_vertices_values(io_luaState)))
			{
				goto OnExit;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			std::cerr << "The value at \"" << key << "\" must be a table "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
			goto OnExit;
		}

	OnExit:

		// Pop the textures table
		lua_pop(&io_luaState, 1);

		return result;
	}

	eae6320::cResult LoadTableValues_vertices_values(lua_State& io_luaState)
	{
		// Right now the asset table is at -2
		// and the vertices table is at -1.
		// NOTE, however, that it doesn't matter to me in this function
		// that the asset table is at -2.
		// Because I've carefully called a new function for every "stack level"
		// The only thing I care about is that the textures table that I care about
		// is at the top of the stack.
		// As long as I make sure that when I leave this function it is _still_
		// at -1 then it doesn't matter to me at all what is on the stack below it.

		auto result = eae6320::Results::Success;

		//Iterating through every vertex value
		s_vertexCount = static_cast<unsigned int>(luaL_len(&io_luaState, -1));
		for (unsigned int i = 1; i <= s_vertexCount; ++i)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);
			
			{
			//Get the position table
			constexpr auto* const key = "position";
			lua_pushstring(&io_luaState, key);
			lua_gettable(&io_luaState, -2);

				{
					//Get x
					constexpr auto* const key2 = "x";
					lua_pushstring(&io_luaState, key2);
					lua_gettable(&io_luaState, -2);

					s_vertexData[i - 1].x = static_cast<float>(lua_tonumber(&io_luaState, -1));

					lua_pop(&io_luaState, 1);

					//Get y
					constexpr auto* const key3 = "y";
					lua_pushstring(&io_luaState, key3);
					lua_gettable(&io_luaState, -2);

					s_vertexData[i - 1].y = static_cast<float>(lua_tonumber(&io_luaState, -1));

					lua_pop(&io_luaState, 1);

					//Get z
					constexpr auto* const key4 = "z";
					lua_pushstring(&io_luaState, key4);
					lua_gettable(&io_luaState, -2);

					s_vertexData[i - 1].z = static_cast<float>(lua_tonumber(&io_luaState, -1));

					lua_pop(&io_luaState, 1);

				}
				lua_pop(&io_luaState, 1);
			}

			{
				//Get color table 

				constexpr auto* const key = "color";
				lua_pushstring(&io_luaState, key);
				lua_gettable(&io_luaState, -2);

				{
					//Get r 
					constexpr auto* const key2 = "r";
					lua_pushstring(&io_luaState, key2);
					lua_gettable(&io_luaState, -2);

					s_vertexData[i - 1].r = static_cast<uint8_t>(lua_tonumber(&io_luaState, -1) * 255.0f);

					lua_pop(&io_luaState, 1);

					//Get g 
					constexpr auto* const key3 = "g";
					lua_pushstring(&io_luaState, key3);
					lua_gettable(&io_luaState, -2);

					s_vertexData[i - 1].g = static_cast<uint8_t>(lua_tonumber(&io_luaState, -1) * 255.0f);

					lua_pop(&io_luaState, 1);

					//Get b 
					constexpr auto* const key4 = "b";
					lua_pushstring(&io_luaState, key4);
					lua_gettable(&io_luaState, -2);

					s_vertexData[i - 1].b = static_cast<uint8_t>(lua_tonumber(&io_luaState, -1) * 255.0f);

					lua_pop(&io_luaState, 1);

					//Get a 
					constexpr auto* const key5 = "a";
					lua_pushstring(&io_luaState, key5);
					lua_gettable(&io_luaState, -2);

					s_vertexData[i - 1].a = static_cast<uint8_t>(lua_tonumber(&io_luaState, -1) * 255.0f);

					lua_pop(&io_luaState, 1);
				
				}
				lua_pop(&io_luaState, 1);
			}


			lua_pop(&io_luaState, 1);
		}

		return result;
	}

	eae6320::cResult LoadTableValues_indices(lua_State& io_luaState)
	{
		auto result = eae6320::Results::Success;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "indices" table will be at -1:
		constexpr auto* const key = "indices";
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			if (!(result = LoadTableValues_indices_values(io_luaState)))
			{
				goto OnExit;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			std::cerr << "The value at \"" << key << "\" must be a table "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
			goto OnExit;
		}

	OnExit:

		// Pop the indices table
		lua_pop(&io_luaState, 1);

		return result;
	}

	eae6320::cResult LoadTableValues_indices_values(lua_State& io_luaState)
	{
		// Right now the indices table is at -1.

		auto result = eae6320::Results::Success;

		//Iterating through every vertex value
		s_indexCount = static_cast<unsigned int>(luaL_len(&io_luaState, -1));
		for (unsigned int i = 1; i <= s_indexCount; ++i)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);

			s_indexData[i - 1] = static_cast<uint16_t>(lua_tonumber(&io_luaState, -1));

			// Pop the indices table
			lua_pop(&io_luaState, 1);
		}

		return result;
	}

	eae6320::cResult LoadAsset(const char* const i_path)
	{
		auto result = eae6320::Results::Success;

		// Create a new Lua state
		lua_State* luaState = nullptr;
		{
			luaState = luaL_newstate();
			if (!luaState)
			{
				result = eae6320::Results::OutOfMemory;
				std::cerr << "Failed to create a new Lua state" << std::endl;
				goto OnExit;
			}
		}

		// Load the asset file as a "chunk",
		// meaning there will be a callable function at the top of the stack
		const auto stackTopBeforeLoad = lua_gettop(luaState);
		{
			const auto luaResult = luaL_loadfile(luaState, i_path);
			if (luaResult != LUA_OK)
			{
				result = eae6320::Results::Failure;
				std::cerr << lua_tostring(luaState, -1) << std::endl;
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}
		// Execute the "chunk", which should load the asset
		// into a table at the top of the stack
		{
			constexpr int argumentCount = 0;
			constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
			constexpr int noMessageHandler = 0;
			const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
			if (luaResult == LUA_OK)
			{
				// A well-behaved asset file will only return a single value
				const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
				if (returnedValueCount == 1)
				{
					// A correct asset file _must_ return a table
					if (!lua_istable(luaState, -1))
					{
						result = eae6320::Results::InvalidFile;
						std::cerr << "Asset files must return a table (instead of a " <<
							luaL_typename(luaState, -1) << ")" << std::endl;
						// Pop the returned non-table value
						lua_pop(luaState, 1);
						goto OnExit;
					}
				}
				else
				{
					result = eae6320::Results::InvalidFile;
					std::cerr << "Asset files must return a single table (instead of " <<
						returnedValueCount << " values)" << std::endl;
					// Pop every value that was returned
					lua_pop(luaState, returnedValueCount);
					goto OnExit;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << lua_tostring(luaState, -1) << std::endl;
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		result = LoadTableValues(*luaState);

		// Pop the table
		lua_pop(luaState, 1);

	OnExit:

		if (luaState)
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack,
			// regardless of any errors encountered while loading the file:
			EAE6320_ASSERT(lua_gettop(luaState) == 0);

			lua_close(luaState);
			luaState = nullptr;
		}

		return result;
	}
}



// Static Data Initialization
//===========================

eae6320::Assets::cManager<eae6320::Graphics::Mesh> eae6320::Graphics::Mesh::s_manager;

// Implementation
//===============

eae6320::cResult eae6320::Graphics::Mesh::Load(const char* const i_path, Mesh*& o_mesh)
{

	auto result = eae6320::Results::Success;

    const char* const path = i_path;
	if (!(result = LoadAsset(path)))
	{
		return result;
	}

	Mesh* newMesh = nullptr;

	// Allocate a new mesh
	{
		newMesh = new Mesh();
		if (!newMesh)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
			Logging::OutputError("Failed to allocate memory for the mesh");
			goto OnExit;
		}
	}



	// The rest of the data is pixel information that is used to create platform-specific textures
	{
		
		if (!(result = newMesh->Initialize(s_vertexCount, s_vertexData, s_indexCount,s_indexData)))
		{
			EAE6320_ASSERTF(false, "Initialization of new texture failed");
			goto OnExit;
		}
	}

OnExit:

	if (result)
	{
		EAE6320_ASSERT(newMesh);
		o_mesh = newMesh;
	}
	else
	{
		if (newMesh)
		{
			newMesh->DecrementReferenceCount();
			newMesh = nullptr;
		}
		o_mesh = nullptr;
	}

	return result;
}


void eae6320::Graphics::Mesh::Delete(eae6320::Graphics::Mesh* &o_mesh)
{
	o_mesh->DecrementReferenceCount();
	o_mesh = nullptr;
}

eae6320::Graphics::Mesh::Mesh()
{

}

eae6320::Graphics::Mesh::~Mesh()
{
	CleanUp();
}



