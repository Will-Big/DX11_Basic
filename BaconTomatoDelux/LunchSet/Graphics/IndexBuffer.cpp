#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
}

void IndexBuffer::Create(ComPtr<ID3D11Device> device, const std::vector<uint32_t>& indices)
{
	m_IndexCount = static_cast<uint32_t>(indices.size());

	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = sizeof(uint32_t) * m_IndexCount;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = indices.data();

	HR_T(device->CreateBuffer(&desc, &data, m_IndexBuffer.GetAddressOf()));
}
