#include "pch.h"
#include "VertexShader.h"

VertexShader::VertexShader(ComPtr<ID3D11Device> device)
	: Shader(device)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const WCHAR* szFileName, LPCSTR szEntryPoint, D3D_SHADER_MACRO* defines)
{
	CompileShaderFromFile(szFileName, szEntryPoint, "vs_5_0", defines);
	HR_T(m_Device->CreateVertexShader(
		m_Blob->GetBufferPointer(), 
		m_Blob->GetBufferSize(), 
		nullptr, 
		m_VertexShader.GetAddressOf()
	));
}