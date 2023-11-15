#pragma once

/**
 * 매 객체마다 변화될 상수 버퍼
 */

__declspec(align(16))
struct TransformData
{
	Matrix World;

	const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};

__declspec(align(16))
struct MaterialData
{
    uint32_t ShaderScope;
    float SpecularPower;
    float OpacityPower;

    const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};