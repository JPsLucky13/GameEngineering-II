--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	meshes =
	{
		"Meshes/MovableMesh.lua",
		"Meshes/StaticMesh.lua",
		"Meshes/SecondStaticMesh.lua",
		"Meshes/PumpkinMesh.lua",
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
	},
	textures =
	{
		-- EAE6320_TODO:
		-- These are some examples with my disk layout.
		-- You need to update the paths to match your content
		-- and then add the other textures that you use.
		"Textures/texture1.png",
		"Textures/texture2.png",
		"Textures/texture3.png",
		"Textures/texture4.png",
		"Textures/texture5.png",
		"Textures/texture6.png",
		"Textures/Pumpkin.png",
	},
}
