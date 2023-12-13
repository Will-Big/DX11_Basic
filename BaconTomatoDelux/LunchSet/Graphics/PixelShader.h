#pragma once
#include "Shader.h"

class PixelShader : public Shader
{
public:
	PixelShader(ComPtr<ID3D11Device> device);
	~PixelShader() override;
	
	void Create(const WCHAR* szFileName, LPCSTR szEntryPoint, D3D_SHADER_MACRO* defines) override;

	ComPtr<ID3D11PixelShader> GetComPtr() { return m_PixelShader; }

private:
	ComPtr<ID3D11PixelShader> m_PixelShader;
};

