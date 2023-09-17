#pragma once

#include "Graphics.h"

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:

private:
	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();

private:
	HWND _hwnd;

	shared_ptr<Graphics> _graphics;

private:
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	shared_ptr<VertexShader> _vertexShader;
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;
	shared_ptr<PixelShader> _pixelShader;

	// SRV
	shared_ptr<Texture> _texture1;

	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	ComPtr<ID3D11BlendState> _blendState = nullptr;
	// [ CPU<->RAM ] [GPU<->VRAM]
private:
	// SRT
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	Vec3 _localPosition = {0.f, 0.f, 0.f};
	Vec3 _localRotation = {0.f, 0.f, 0.f};
	Vec3 _localScale = {1.f, 1.f, 1.f};
};

