#pragma once

class Sampler
{
public:
	Sampler(ComPtr<ID3D11Device> device);
	~Sampler();

	void Create();
	ComPtr<ID3D11SamplerState> GetComPtr() { return m_SamplerState; }

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11SamplerState> m_SamplerState;
};
