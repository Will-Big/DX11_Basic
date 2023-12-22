#pragma once

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer() = default;

	void Create(ComPtr<ID3D11Device> device, const std::vector<uint32_t>& indices);
	ComPtr<ID3D11Buffer> GetComPtr() { return m_IndexBuffer; }
	uint32_t GetIndexCount() { return m_IndexCount; }

private:
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	uint32_t m_IndexCount = 0;
};

