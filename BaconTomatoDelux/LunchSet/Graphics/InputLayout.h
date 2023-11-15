#pragma once
#include "Helper.h"

class InputLayout
{
public:
	InputLayout(ComPtr<ID3D11Device> device, ComPtr<ID3DBlob> blob);
	~InputLayout();

	template<typename T, typename = decltype(T::desc)>
	void Create();

	ComPtr<ID3D11InputLayout> GetInputLayout() { return m_InputLayout; }

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3DBlob> m_Blob;
	ComPtr<ID3D11InputLayout> m_InputLayout;
};

template <typename T, typename>
void InputLayout::Create()
{
	auto data = T::desc.data();
	auto size = T::desc.size();

	HRESULT hr = m_Device->CreateInputLayout(T::desc.data(), T::desc.size(),
		m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), m_InputLayout.GetAddressOf());

	HR_T(hr);
}
