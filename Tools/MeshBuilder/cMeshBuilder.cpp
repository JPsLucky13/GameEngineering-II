// Include Files
//==============

#include "cMeshBuilder.h"
#include <Engine\Platform\Platform.h>
#include <Tools\AssetBuildLibrary\Functions.h>
#include <External/Lua/Includes.h>
#include <Engine\Graphics\VertexFormats.h>
#include <fstream>

// Helper Function Declarations
//=============================

namespace
{
	uint16_t s_vertexCount;
	uint16_t s_indexCount;
	eae6320::Graphics::VertexFormats::sMesh  * s_vertexData = NULL;
	uint16_t * s_indexData = NULL;

	eae6320::cResult LoadTableValues(lua_State& io_luaState,const char* const i_path);

	eae6320::cResult LoadTableValues_vertices(lua_State& io_luaState, const char* const i_path);
	eae6320::cResult LoadTableValues_vertices_values(lua_State& io_luaState, const char* const i_path);

	eae6320::cResult LoadTableValues_indices(lua_State& io_luaState, const char* const i_path);
	eae6320::cResult LoadTableValues_indices_values(lua_State& io_luaState, const char* const i_path);

	eae6320::cResult LoadAsset(const char* const i_path);
}

// Interface
//==========


// Helper Function Definitions
//============================

namespace
{
	eae6320::cResult LoadTableValues(lua_State& io_luaState, const char* const i_path)
	{
		auto result = eae6320::Results::Success;

		if (!(result = LoadTableValues_vertices(io_luaState,i_path)))
		{
			return result;
		}
		if (!(result = LoadTableValues_indices(io_luaState,i_path)))
		{
			return result;
		}

		return result;
	}

	eae6320::cResult LoadTableValues_vertices(lua_State& io_luaState, const char* const i_path)
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
			if (!(result = LoadTableValues_vertices_values(io_luaState,i_path)))
			{
				goto OnExit;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			//EAE6320_ASSERT(result);
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"The value must be a table ");
			goto OnExit;
		}

	OnExit:

		// Pop the vertices table
		lua_pop(&io_luaState, 1);

		return result;
	}

	eae6320::cResult LoadTableValues_vertices_values(lua_State& io_luaState, const char* const i_path)
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
		s_vertexCount = static_cast<uint16_t>(luaL_len(&io_luaState, -1));
		s_vertexData = new eae6320::Graphics::VertexFormats::sMesh[s_vertexCount];
		for (uint16_t i = 1; i <= s_vertexCount; ++i)
		{
			lua_pushinteger(&io_luaState, i);
			lua_gettable(&io_luaState, -2);

			{
				//Get the position table
				constexpr auto* const key = "position";
				lua_pushstring(&io_luaState, key);
				lua_gettable(&io_luaState, -2);


				if (!lua_istable(&io_luaState, -1))
				{
					result = eae6320::Results::InvalidFile;
					//EAE6320_ASSERT(result);
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"The value must be a table");
					return result;
				}

				// First, then, we will make sure that a value (_any_ value) existed for the key:
				if (lua_isnil(&io_luaState, -1))
				{

					result = eae6320::Results::InvalidFile;
					//EAE6320_ASSERT(result);
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"No value was found in the asset table");
					lua_pop(&io_luaState, 1);
					// Now the only thing on the stack is the asset table at -1,
					// and the calling function will deal with this
					// (regardless of whether this function succeeds or fails).
					return result;
				}

				{
					//Get x
					constexpr auto* const key2 = "x";
					lua_pushstring(&io_luaState, key2);
					lua_gettable(&io_luaState, -2);


					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}


					s_vertexData[i - 1].x = static_cast<float>(lua_tonumber(&io_luaState, -1));

					lua_pop(&io_luaState, 1);

					//Get y
					constexpr auto* const key3 = "y";
					lua_pushstring(&io_luaState, key3);
					lua_gettable(&io_luaState, -2);

					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}

					s_vertexData[i - 1].y = static_cast<float>(lua_tonumber(&io_luaState, -1));

					lua_pop(&io_luaState, 1);

					//Get z
					constexpr auto* const key4 = "z";
					lua_pushstring(&io_luaState, key4);
					lua_gettable(&io_luaState, -2);

					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}

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

				if (!lua_istable(&io_luaState, -1))
				{
					result = eae6320::Results::InvalidFile;
					//EAE6320_ASSERT(result);
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"The value must be a table");
					return eae6320::Results::InvalidFile;
				}

				// First, then, we will make sure that a value (_any_ value) existed for the key:
				if (lua_isnil(&io_luaState, -1))
				{

					result = eae6320::Results::InvalidFile;
					//EAE6320_ASSERT(result);
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"No value was found in the asset table");
					lua_pop(&io_luaState, 1);
					// Now the only thing on the stack is the asset table at -1,
					// and the calling function will deal with this
					// (regardless of whether this function succeeds or fails).
					return result;
				}

				{
					//Get r 
					constexpr auto* const key2 = "r";
					lua_pushstring(&io_luaState, key2);
					lua_gettable(&io_luaState, -2);


					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}

					s_vertexData[i - 1].r = static_cast<uint8_t>(lua_tonumber(&io_luaState, -1) * 255.0f);

					lua_pop(&io_luaState, 1);

					//Get g 
					constexpr auto* const key3 = "g";
					lua_pushstring(&io_luaState, key3);
					lua_gettable(&io_luaState, -2);


					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}

					s_vertexData[i - 1].g = static_cast<uint8_t>(lua_tonumber(&io_luaState, -1) * 255.0f);

					lua_pop(&io_luaState, 1);

					//Get b 
					constexpr auto* const key4 = "b";
					lua_pushstring(&io_luaState, key4);
					lua_gettable(&io_luaState, -2);


					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}

					s_vertexData[i - 1].b = static_cast<uint8_t>(lua_tonumber(&io_luaState, -1) * 255.0f);

					lua_pop(&io_luaState, 1);

					//Get a 
					constexpr auto* const key5 = "a";
					lua_pushstring(&io_luaState, key5);
					lua_gettable(&io_luaState, -2);


					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}

					s_vertexData[i - 1].a = static_cast<uint8_t>(lua_tonumber(&io_luaState, -1) * 255.0f);

					lua_pop(&io_luaState, 1);

				}
				lua_pop(&io_luaState, 1);
			}

			{
				//Get Texture table 

				constexpr auto* const key = "texture";
				lua_pushstring(&io_luaState, key);
				lua_gettable(&io_luaState, -2);

				if (!lua_istable(&io_luaState, -1))
				{
					result = eae6320::Results::InvalidFile;
					//EAE6320_ASSERT(result);
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a table");
					return eae6320::Results::InvalidFile;
				}

				// First, then, we will make sure that a value (_any_ value) existed for the key:
				if (lua_isnil(&io_luaState, -1))
				{

					result = eae6320::Results::InvalidFile;
					//EAE6320_ASSERT(result);
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "No value was found in the asset table");
					lua_pop(&io_luaState, 1);
					// Now the only thing on the stack is the asset table at -1,
					// and the calling function will deal with this
					// (regardless of whether this function succeeds or fails).
					return result;
				}

				{
					//Get u 
					constexpr auto* const key2 = "u";
					lua_pushstring(&io_luaState, key2);
					lua_gettable(&io_luaState, -2);


					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}

					s_vertexData[i - 1].u = static_cast<float>(lua_tonumber(&io_luaState, -1));

					lua_pop(&io_luaState, 1);

					//Get g 
					constexpr auto* const key3 = "v";
					lua_pushstring(&io_luaState, key3);
					lua_gettable(&io_luaState, -2);


					// First, then, we will make sure that a value (_any_ value) existed for the key:
					if (lua_isnil(&io_luaState, -1))
					{

						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "No value was found in the asset table");
						lua_pop(&io_luaState, 3);
						return result;
					}

					if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
					{
						result = eae6320::Results::InvalidFile;
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a number");
						// Pop the value
						lua_pop(&io_luaState, 3);
						return result;
					}

					s_vertexData[i - 1].v = static_cast<float>(lua_tonumber(&io_luaState, -1));

					lua_pop(&io_luaState, 1);

				}
				lua_pop(&io_luaState, 1);
			}



			lua_pop(&io_luaState, 1);
		}

		return result;
	}

	eae6320::cResult LoadTableValues_indices(lua_State& io_luaState, const char* const i_path)
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
			if (!(result = LoadTableValues_indices_values(io_luaState,i_path)))
			{
				goto OnExit;
			}
		}
		else
		{
			result = result = eae6320::Results::InvalidFile;
			//EAE6320_ASSERT(result);
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_path, "The value must be a table.");
			goto OnExit;
		}

	OnExit:

		// Pop the indices table
		lua_pop(&io_luaState, 1);

		return result;
	}

	eae6320::cResult LoadTableValues_indices_values(lua_State& io_luaState, const char* const i_path)
	{
		// Right now the indices table is at -1.

		auto result = eae6320::Results::Success;

		//Iterating through every vertex value
		s_indexCount = static_cast<uint16_t>(luaL_len(&io_luaState, -1));
		s_indexData = new uint16_t[s_indexCount];
		for (uint16_t i = 1; i <= s_indexCount; ++i)
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
				//EAE6320_ASSERT(result);
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"Failed to create a lua state");
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
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"Failed to load lua file");
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
						//EAE6320_ASSERT(result);
						eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"Asset files must return a table");


						// Pop the returned non-table value
						lua_pop(luaState, 1);
						goto OnExit;
					}
				}
				else
				{
					result = eae6320::Results::InvalidFile;
					//EAE6320_ASSERT(result);
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"Asset files must return a table");
					// Pop every value that was returned
					lua_pop(luaState, returnedValueCount);
					goto OnExit;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_path,"Failed to load lua file");
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		result = LoadTableValues(*luaState, i_path);

		// Pop the table
		lua_pop(luaState, 1);

	OnExit:

		if (luaState)
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack,
			// regardless of any errors encountered while loading the file:
			//EAE6320_ASSERT(lua_gettop(luaState) == 0);

			lua_close(luaState);
			luaState = nullptr;
		}

		return result;
	}
}

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>&)
{
	auto result = eae6320::Results::Success;

	// Write the binary file
	{
		std::string errorMessage;
		// There are many reasons that a source should be rebuilt,
		// and so even if the target already exists it should just be written over
		constexpr bool noErrorIfTargetAlreadyExists = false;
		// Since we rely on timestamps to determine when a target was built
		// its file time should be updated when the source gets copied
		constexpr bool updateTheTargetFileTime = true;

		//Read the lua data from the path source which is a .lua file
		if (!(result = LoadAsset(m_path_source)))
		{
			OutputErrorMessageWithFileInfo(m_path_source, errorMessage.c_str());
			return eae6320::Results::Failure;
		}

		AdjustWindingOrderAndUVs(s_vertexCount, s_vertexData, s_indexCount, s_indexData);

		std::ofstream fout(m_path_target, std::ofstream::binary);
		
		if (fout.is_open())
		{
			fout.write(reinterpret_cast<char *>(&s_vertexCount), sizeof(uint16_t));
			fout.write(reinterpret_cast<char *>(s_vertexData), sizeof(eae6320::Graphics::VertexFormats::sMesh) * s_vertexCount);
			fout.write(reinterpret_cast<char *>(&s_indexCount), sizeof(uint16_t));
			fout.write(reinterpret_cast<char *>(s_indexData), sizeof(uint16_t) * s_indexCount);
		}
		
		fout.close();
	
		// Delete the arrays of vertices and indices
		delete[]s_vertexData;
		s_vertexData = NULL;
		delete[]s_indexData;
		s_indexData = NULL;

		return result;
	}
}