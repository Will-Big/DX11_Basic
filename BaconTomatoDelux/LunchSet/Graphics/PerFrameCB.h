#pragma once
/**
 * 매 프레임마다 변화될 상수 버퍼
 */

__declspec(align(16))
struct VSCameraData
{
	Matrix View;

	const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};

__declspec(align(16))
struct PSCameraData
{
	Vector4 Position;

	const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};

__declspec(align(16))
struct PSLightData
{
	Vector4 LightDirection;
	Vector4 LightColor;
	float LightIntensity;

	const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};