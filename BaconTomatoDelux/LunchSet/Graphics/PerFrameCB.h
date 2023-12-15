#pragma once
/**
 * 매 프레임마다 변화될 상수 버퍼
 */

__declspec(align(16))
struct CameraData
{
	Matrix View;
	Matrix Projection;
	Vector4 CameraPosition;

	const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};

__declspec(align(16))
struct LightData
{
	Vector4 LightDirection;
	Vector4 LightColor;
	float LightIntensity;

	const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};