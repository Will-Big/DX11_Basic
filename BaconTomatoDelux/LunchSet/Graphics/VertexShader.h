#pragma once
#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(ComPtr<ID3D11Device> device);
	~VertexShader() override = default;

	void Create(const WCHAR* szFileName, LPCSTR szEntryPoint, D3D_SHADER_MACRO* defines) override;

	ComPtr<ID3D11VertexShader> GetComPtr() { return m_VertexShader; }

private:
	ComPtr<ID3D11VertexShader> m_VertexShader;
};

/*
Blob (Binary Large Object)
	�뵵: �ַ� ���̴� ������ ����� ���� �����͸� �ӽ÷� �����ϴ� �� ���˴ϴ�.
	Ư¡: �޸𸮿� ���� ������ ������ �����ϸ�, ũ�Ⱑ �������� ����� �� �ֽ��ϴ�.
	����: HLSL ���̴��� �������� ���� ����Ʈ�ڵ带 ������ �� ID3DBlob �������̽��� ���˴ϴ�.

Buffer
	�뵵: �׷���ī���� �޸𸮿� �����͸� �����ϴ� �� ���˴ϴ�. ���� ���, ���ؽ�, �ε���, ��� ���� �����͸� �����մϴ�.
	Ư¡: GPU�� ���� ������ �� ������, ������ ���������ο��� ���˴ϴ�.
	����: ���ؽ� �����͸� �����ϴ� ID3D11Buffer �������̽��� �ֽ��ϴ�.
*/