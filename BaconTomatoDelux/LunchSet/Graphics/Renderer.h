#pragma once
#include "ConstantBuffer.h"
#include "Texture.h"

// CB
struct CameraData;
struct LightData;
struct TransformData;
struct MatrixPallete;

class VertexBuffer;
class IndexBuffer;
class Texture;
class Shader;
class VertexShader;
class PixelShader;
class InputLayout;

// 상수버퍼의 변화가 없었다면 nullptr 로 넘기기
struct FrameSettings
{
	const CameraData* camera = nullptr;
	const LightData* light = nullptr;
};

struct ObjectSettings
{
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;
	std::array<std::shared_ptr<Texture>, btdTextureType_END> textures;

	const TransformData* transform = nullptr;
};

struct MatrixPalleteSettings
{
	const MatrixPallete* pallete;
};

class Renderer
{
public:
	Renderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~Renderer();

	void SetPerFrame(const FrameSettings& settings);
	void SetPerObject(const ObjectSettings& settings);
	void SetMatrixPallete(const MatrixPalleteSettings& settings);

	// todo : SetPerObject 로 이동
	void SetInputLayout(std::shared_ptr<InputLayout> layout);
	
	template <typename T>
	void SetShader(std::shared_ptr<T> shader);
	void Draw();

private:
	template<typename T, typename = decltype(T::bindings)>
	void SetConstantBuffer(ConstantBuffer<T>& buffer);

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	uint32_t m_IndexCount = 0;
};

template <typename T, typename>
void Renderer::SetConstantBuffer(ConstantBuffer<T>& buffer)
{
	auto binder = [this, &buffer](const std::pair<btdShaderScope, int>& binding)
	{
		switch (auto [scope, slotNum] = binding; scope)
		{
		case btdShaderScope_Vertex:
			m_DeviceContext->VSSetConstantBuffers(slotNum, 1, buffer.GetComPtr().GetAddressOf());
			break;
		case btdShaderScope_Pixel:
			m_DeviceContext->PSSetConstantBuffers(slotNum, 1, buffer.GetComPtr().GetAddressOf());
			break;
		default:
			break;
		}
	};

	for (const std::pair<btdShaderScope, int>& binding : T::bindings)
	{
		binder(binding);
	}
}

template <typename T>
void Renderer::SetShader(std::shared_ptr<T> shader)
{
	static_assert(std::is_base_of_v<Shader, T>, "T must be a derived class of Shader");

	if constexpr (std::is_base_of_v<VertexShader, T>)
	{
		m_DeviceContext->VSSetShader(shader->GetComPtr().Get(), nullptr, 0);
	}
	else if constexpr (std::is_base_of_v<PixelShader, T>)
	{
		m_DeviceContext->PSSetShader(shader->GetComPtr().Get(), nullptr, 0);
	}
}

