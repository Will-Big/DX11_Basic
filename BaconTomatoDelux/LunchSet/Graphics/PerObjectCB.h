#pragma once


/**
 * 그래픽 파이프라인 관련 정보
 */

class VertexShader;
class PixelShader;
class InputLayout;
class Sampler;

struct PipelineData
{
    std::shared_ptr<VertexShader> vertexShader;
    std::shared_ptr<PixelShader> pixelShader;
    std::shared_ptr<InputLayout> inputLayout;
    std::shared_ptr<Sampler> sampler;
};

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

__declspec(align(16))
struct MatrixPallete
{
    Matrix Array[128];

    const static std::vector<std::pair<btdShaderScope, uint32_t>> bindings;
};