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
	용도: 주로 셰이더 컴파일 결과나 파일 데이터를 임시로 저장하는 데 사용됩니다.
	특징: 메모리에 대한 저수준 접근이 가능하며, 크기가 동적으로 변경될 수 있습니다.
	예시: HLSL 셰이더를 컴파일한 후의 바이트코드를 저장할 때 ID3DBlob 인터페이스가 사용됩니다.

Buffer
	용도: 그래픽카드의 메모리에 데이터를 저장하는 데 사용됩니다. 예를 들어, 버텍스, 인덱스, 상수 등의 데이터를 저장합니다.
	특징: GPU가 직접 접근할 수 있으며, 렌더링 파이프라인에서 사용됩니다.
	예시: 버텍스 데이터를 저장하는 ID3D11Buffer 인터페이스가 있습니다.
*/