#include "pch.h"
#include "PerObjectCB.h"

const std::vector<std::pair<btdShaderScope, uint32_t>> TransformData::bindings
{
	{btdShaderScope_Vertex, 1},
};

const std::vector<std::pair<btdShaderScope, uint32_t>> MaterialData::bindings
{
	{btdShaderScope_Pixel, 1},
};
