#include "pch.h"
#include "PixelShader.h"

PixelShader::PixelShader(ComPtr<ID3D11Device> device)
	: Shader(device)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const WCHAR* szFileName, LPCSTR szEntryPoint, D3D_SHADER_MACRO* defines)
{
	CompileShaderFromFile(szFileName, szEntryPoint, "ps_5_0", defines);
	HR_T(m_Device->CreatePixelShader(
		m_Blob->GetBufferPointer(), 
		m_Blob->GetBufferSize(), 
		nullptr, 
		m_PixelShader.GetAddressOf()
	));
}