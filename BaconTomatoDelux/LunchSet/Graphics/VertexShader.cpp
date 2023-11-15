#include "pch.h"
#include "VertexShader.h"

VertexShader::VertexShader(ComPtr<ID3D11Device> device)
	: Shader(device)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
{
	CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel);
	HR_T(m_Device->CreateVertexShader(
		m_Blob->GetBufferPointer(), 
		m_Blob->GetBufferSize(), 
		nullptr, 
		m_VertexShader.GetAddressOf()
	));
}