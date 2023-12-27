#pragma once

enum btdShaderScope
{
	btdShaderScope_VERTEX,
	btdShaderScope_PIXEL,

	btdShaderScope_END
};

enum btdTextureType
{
	btdTextureType_DIFFUSE = 0,
	btdTextureType_NORMALS,
	btdTextureType_SPECULAR,
	btdTextureType_OPACITY,
	btdTextureType_EMISSIVE,
	btdTextureType_METALNESS,
	btdTextureType_ROUGHNESS,
	btdTextureType_END,
};