﻿#include "pch.h"
#include "PerObjectCB.h"

const std::vector<std::pair<btdShaderScope, uint32_t>> VSObjectData::bindings
{
	{btdShaderScope_VERTEX, 2},
};

const std::vector<std::pair<btdShaderScope, uint32_t>> VsMatrixPallete::bindings
{
	{btdShaderScope_VERTEX, 3},
};

const std::vector<std::pair<btdShaderScope, uint32_t>> PsMaterialData::bindings
{
	{btdShaderScope_PIXEL, 2},
};

