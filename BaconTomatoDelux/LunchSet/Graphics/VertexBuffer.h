#pragma once
#include "Helper.h"

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	template<typename T, typename = decltype(T::desc)>
	void Create(ComPtr<ID3D11Device> device, const std::vector<T>& vertices);

	ComPtr<ID3D11Buffer> GetComPtr() { return m_VertexBuffer; }
	uint32_t GetStride() { return m_Stride; }
	uint32_t GetOffset() { return m_Offset; }

private:
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	uint32_t m_Stride = 0;
	uint32_t m_Offset = 0;
};

template <typename T, typename>
void VertexBuffer::Create(ComPtr<ID3D11Device> device, const std::vector<T>& vertices)
{
	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vertices.data();

	m_Stride = sizeof(T);
	m_Offset = 0;

	HRESULT hr = device->CreateBuffer(&desc, &data, m_VertexBuffer.GetAddressOf());

	HR_T(hr);
}

