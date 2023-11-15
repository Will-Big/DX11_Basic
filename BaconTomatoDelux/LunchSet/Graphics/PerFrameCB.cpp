﻿#include "pch.h"
#include "PerFrameCB.h"

const std::vector<std::pair<btdShaderScope, uint32_t>> CameraData::bindings
{
	{btdShaderScope_Vertex, 0},
};

const std::vector<std::pair<btdShaderScope, uint32_t>> LightData::bindings
{
	{btdShaderScope_Pixel, 0},
};