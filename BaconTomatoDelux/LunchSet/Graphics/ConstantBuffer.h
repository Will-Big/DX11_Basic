#pragma once
#include "Forward.h"
#include "Helper.h"

template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~ConstantBuffer();

	void Update(const T& desc);
	void Update(const T* desc);

	ComPtr<ID3D11Buffer> GetComPtr() { return m_Buffer; }
	
private:
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ComPtr<ID3D11Buffer> m_Buffer;
};

template <typename T>
ConstantBuffer<T>::ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: m_DeviceContext(deviceContext)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(T);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HR_T(device->CreateBuffer(&desc, nullptr, &m_Buffer));
}

template <typename T>
ConstantBuffer<T>::~ConstantBuffer()
{
}

template <typename T>
void ConstantBuffer<T>::Update(const T& desc)
{
	m_DeviceContext->UpdateSubresource(m_Buffer.Get(), 0, nullptr, &desc, 0, 0);
}

template <typename T>
void ConstantBuffer<T>::Update(const T* desc)
{
	m_DeviceContext->UpdateSubresource(m_Buffer.Get(), 0, nullptr, desc, 0, 0);
}
