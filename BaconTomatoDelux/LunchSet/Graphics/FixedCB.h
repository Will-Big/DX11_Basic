#pragma once

__declspec(align(16))
struct VSFixedData
{
	Matrix Projection;

	const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};