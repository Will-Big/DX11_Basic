#include "pch.h"
#include "PerFrameCB.h"

const std::vector<std::pair<btdShaderScope, uint32_t>> VSCameraData::bindings
{
	{btdShaderScope_Vertex, 1},
};

const std::vector<std::pair<btdShaderScope, uint32_t>> PSCameraData::bindings
{
	{btdShaderScope_Pixel, 0},
};

const std::vector<std::pair<btdShaderScope, uint32_t>> PSLightData::bindings
{
	{btdShaderScope_Pixel, 1},
};