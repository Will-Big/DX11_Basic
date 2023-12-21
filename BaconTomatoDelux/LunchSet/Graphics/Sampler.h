#pragma once

class Sampler
{
public:
	Sampler(ComPtr<ID3D11Device> device);
	~Sampler();

	ComPtr<ID3D11SamplerState> GetComPtr() { return m_SamplerState; }

private:
	void Create(ComPtr<ID3D11Device> device);

	ComPtr<ID3D11SamplerState> m_SamplerState;
};
