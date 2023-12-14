﻿#pragma once

enum btdTextureType
{
	btdTextureType_DIFFUSE,
	btdTextureType_NORMALS,
	btdTextureType_SPECULAR,
	btdTextureType_OPACITY,
	btdTextureType_EMISSIVE,
	btdTextureType_METALNESS,
	btdTextureType_ROUGHNESS,
	btdTextureType_END,
};

/**
 * \brief	: Texture 의 정보와 데이터를 관리하는 클래스
 * \info	: struct 처럼 사용(일부 public member var)
 */
class Texture
{
public:
	Texture(ComPtr<ID3D11Device> device, std::wstring_view path, btdTextureType type);
	Texture(ComPtr<ID3D11ShaderResourceView> shaderResourceView, btdTextureType type);
	~Texture();

	ComPtr<ID3D11ShaderResourceView> GetComPtr() { return m_ShaderResourceView; }

private:
	void Create(ComPtr<ID3D11Device> device, std::wstring_view path);

public:
	btdTextureType type;
	std::wstring name;

private:
	ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
};
