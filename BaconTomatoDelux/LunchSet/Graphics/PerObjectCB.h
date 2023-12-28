#pragma once
/**
 * 매 객체마다 변화될 상수 버퍼
 */

__declspec(align(16))
struct VSObjectData
{
	Matrix World;

	const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};

__declspec(align(16))
struct VsMatrixPallete
{
    std::array<Matrix, 128> Array;

    const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};

__declspec(align(16))
struct PsMaterialData
{
    uint32_t textureBitmask;
    float SpecularPower;
    float OpacityPower;

    const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};

