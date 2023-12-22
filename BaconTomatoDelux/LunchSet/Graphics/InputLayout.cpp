#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout(ComPtr<ID3D11Device> device, ComPtr<ID3DBlob> blob)
	: m_Device(device), m_Blob(blob)
{
}