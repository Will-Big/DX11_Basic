#include "pch.h"
#include "PixelShader.h"

PixelShader::PixelShader(ComPtr<ID3D11Device> device)
	: Shader(device)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{
	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel);
	HR_T(m_Device->CreatePixelShader(
		m_Blob->GetBufferPointer(), 
		m_Blob->GetBufferSize(), 
		nullptr, 
		m_PixelShader.GetAddressOf()
	));
}