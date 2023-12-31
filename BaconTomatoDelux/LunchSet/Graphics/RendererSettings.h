﻿#pragma once
#include "Texture.h"
#include "PerObjectCB.h"

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

// 객체가 직접 Draw Call 을 하는게 아니라, Render Queue 에 Graphic Resources 를 넣고
// Render Queue 를 정렬하여 적절한 순서대로 렌더링한다
struct StaticRenderQueueSettings
{
	std::shared_ptr<InputLayout> inputLayout;
	std::shared_ptr<VertexShader> vertexShader; // 정렬 기준 2
	std::shared_ptr<PixelShader> pixelShader;

	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;
	std::array<std::shared_ptr<Texture>, btdTextureType_END>* textures = nullptr; // 정렬 기준 1

	VSObjectData transform;
};

struct SkinnedRenderQueueSettings
{
	std::shared_ptr<InputLayout> inputLayout;
	std::shared_ptr<VertexShader> vertexShader; // 정렬 기준 2
	std::shared_ptr<PixelShader> pixelShader;

	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;
	std::array<std::shared_ptr<Texture>, btdTextureType_END>* textures = nullptr; // 정렬 기준 1

	std::shared_ptr<VsMatrixPallete> pallete;
	std::shared_ptr<VSObjectData> transform;
};

// 상수버퍼의 변화가 없었다면 nullptr 로 넘기기
struct FixedSettings
{
	const VSFixedData* vsFixed = nullptr;
};

struct FrameSettings
{
	const VSCameraData* vsCamera = nullptr;
	const PSCameraData* psCamera = nullptr;
	const PSLightData* light = nullptr;
};