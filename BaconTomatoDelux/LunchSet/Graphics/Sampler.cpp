#include "pch.h"
#include "Sampler.h"

Sampler::Sampler(ComPtr<ID3D11Device> device)
	: m_Device(device)
{
}

Sampler::~Sampler()
{
}

void Sampler::Create()
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HR_T(m_Device->CreateSamplerState(&samplerDesc, m_SamplerState.GetAddressOf()));
}
