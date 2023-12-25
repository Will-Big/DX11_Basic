#pragma once
#include "Texture.h"

// CB
struct VSFixedData;
struct VSCameraData;
struct PSCameraData;
struct PSLightData;
struct VSObjectData;
struct VsMatrixPallete;

class VertexBuffer;
class IndexBuffer;
class Shader;
class VertexShader;
class PixelShader;
class InputLayout;

// 객체가 직접 Draw Call 을 하는게 아니라, Render Queue 에 GraphicResources 구조체를 넣고
// Render Queue 를 정렬하여 적절한 순서대로 렌더링한다
struct RenderQueueSettings
{
	const std::shared_ptr<InputLayout> inputLayout;
	const std::shared_ptr<VertexShader> vertexShader;
	const std::shared_ptr<PixelShader> pixelShader;

	const std::shared_ptr<VertexBuffer> vertexBuffer;
	const std::shared_ptr<IndexBuffer> indexBuffer;
	const std::array<std::shared_ptr<Texture>, btdTextureType_END> textures; // 정렬 기준

	const VSObjectData* transform = nullptr;
};

struct FixedSettings
{
	const VSFixedData* vsFixed = nullptr;
};

// 상수버퍼의 변화가 없었다면 nullptr 로 넘기기
struct FrameSettings
{
	const VSCameraData* vsCamera = nullptr;
	const PSCameraData* psCamera = nullptr;
	const PSLightData* light = nullptr;
};

struct ObjectSettings
{
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;
	std::array<std::shared_ptr<Texture>, btdTextureType_END> textures;

	const VSObjectData* transform = nullptr;
};

struct MatrixPalleteSettings
{
	const VsMatrixPallete* pallete;
};