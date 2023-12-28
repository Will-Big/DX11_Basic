#pragma once
#include "ConstantBuffer.h"
#include "RendererSettings.h"

class Graphics;

class Renderer
{
public:
	Renderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~Renderer();

	void SetFixed(const FixedSettings& settings);
	void SetPerFrame(const FrameSettings& settings);

	void AddRenderQueue(const StaticRenderQueueSettings& settings);
	void AddRenderQueue(const SkinnedRenderQueueSettings& settings);
	void DrawAllQueue();

private:
	template<typename T, typename = decltype(T::bindings)>
	void SetConstantBuffer(ConstantBuffer<T>& buffer);

	void SortRenderQueue();
	void DrawStaticQueue();
	void DrawSkinnedQueue();

	// todo : SetPerObject ∑Œ ¿Ãµø
	void SetInputLayout(std::shared_ptr<InputLayout> layout);

	template <typename T>
	void SetShader(std::shared_ptr<T> shader);
	void Draw();

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	uint32_t m_IndexCount = 0;

	std::vector<StaticRenderQueueSettings> m_StaticRenderQueue;
	std::vector<SkinnedRenderQueueSettings> m_SkinnedRenderQueue;
};

template <typename T, typename>
void Renderer::SetConstantBuffer(ConstantBuffer<T>& buffer)
{
	auto binder = [this, &buffer](const std::pair<btdShaderScope, int>& binding)
	{
		switch (auto [scope, slotNum] = binding; scope)
		{
		case btdShaderScope_VERTEX:
			m_DeviceContext->VSSetConstantBuffers(slotNum, 1, buffer.GetComPtr().GetAddressOf());
			break;
		case btdShaderScope_PIXEL:
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

