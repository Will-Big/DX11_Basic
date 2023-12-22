#pragma once

class Shader
{
public:
	Shader(ComPtr<ID3D11Device> device);
	virtual ~Shader() = default;

	virtual void Create(const WCHAR* szFileName, LPCSTR szEntryPoint, D3D_SHADER_MACRO* defines) = 0;

	ComPtr<ID3DBlob> GetBlob() { return m_Blob; }

protected:
	void CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, const D3D_SHADER_MACRO* defines = nullptr);

	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3DBlob> m_Blob;
};

