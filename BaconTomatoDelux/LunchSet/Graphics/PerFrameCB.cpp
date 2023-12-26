#include "pch.h"
#include "PerFrameCB.h"

const std::vector<std::pair<btdShaderScope, uint32_t>> VSCameraData::bindings
{
	{btdShaderScope_VERTEX, 1},
};

const std::vector<std::pair<btdShaderScope, uint32_t>> PSCameraData::bindings
{
	{btdShaderScope_PIXEL, 0},
};

const std::vector<std::pair<btdShaderScope, uint32_t>> PSLightData::bindings
{
	{btdShaderScope_PIXEL, 1},
};