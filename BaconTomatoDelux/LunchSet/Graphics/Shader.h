#pragma once

class Shader
{
public:
	Shader(ComPtr<ID3D11Device> device);
	virtual ~Shader();

	virtual void Create(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel) = 0;

	ComPtr<ID3DBlob> GetBlob() { return m_Blob; }

protected:
	void CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);

protected:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3DBlob> m_Blob;
};

