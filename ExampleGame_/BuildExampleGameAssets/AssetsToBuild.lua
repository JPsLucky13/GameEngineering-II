--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	meshes =
	{
		"Meshes/SpacePaddle.lua",
		"Meshes/Ball.lua",
		"Meshes/SpaceBrick.lua",
	},
	shaders =
	{
		-- EAE6320_TODO:
		-- These are some examples showing how to use the optional argument,
		-- but with my disk layout and file extension.
		-- You need to update the paths to match your content
		-- and then add the other shaders that you use.
		{ path = "Shaders/Vertex/sprite.cgfx", arguments = { "vertex" } },
		{ path = "Shaders/Vertex/mesh.cgfx", arguments = { "vertex" } },
		{ path = "Shaders/Vertex/vertexInputLayout_geometrySprite.cgfx", arguments = { "vertex" } },
		{ path = "Shaders/Vertex/vertexInputLayout_geometryMesh.cgfx", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/sprite1.cgfx", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/sprite2.cgfx", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/mesh.cgfx", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/meshTrans.cgfx", arguments = { "fragment" } },
	},
	textures =
	{
		-- EAE6320_TODO:
		-- These are some examples with my disk layout.
		-- You need to update the paths to match your content
		-- and then add the other textures that you use.
		"Textures/SpacePaddle.png",
		"Textures/Ball.png",
		"Textures/SpaceBrick.png",
		"Textures/youWin.png",
		"Textures/youLose.png",
		"Textures/blank.png",
	},
}
