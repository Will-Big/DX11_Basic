#pragma once
#include "ConstantBuffer.h"
#include "RendererSettings.h"

class Renderer
{
public:
	Renderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~Renderer();

	// Set 관련 함수들은 전부 private 로 변경 예정
	// 대신 그래픽 리소스를 교체하는 방식 및 직접적인 함수명(ex. Camera~)으로 대체
	void SetFixed(const FixedSettings& settings);
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

	// 렌더큐
	//		일단 텍스처를 기준으로 정렬하는 렌더큐
	//		하지만 같은 텍스처를 사용하더라도
	//		다른 쉐이더, 버퍼를 사용하는 특이한 경우가 있을 수 있으므로
	//		이를 나중에 대비하자
	std::vector<RenderQueueSettings> m_RenderQueue;
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

