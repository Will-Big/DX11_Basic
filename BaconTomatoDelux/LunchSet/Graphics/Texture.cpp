#include "pch.h"
#include "Texture.h"

#include <filesystem>

Texture::Texture(ComPtr<ID3D11Device> device, std::wstring_view path, btdTextureType type)
	: type(type), name(path)
{
    Create(device, path);
}

Texture::Texture(ComPtr<ID3D11ShaderResourceView> shaderResourceView, btdTextureType type)
	: type(type), m_ShaderResourceView(shaderResourceView)
{
}

Texture::~Texture()
{
}

void Texture::Create(ComPtr<ID3D11Device> device, std::wstring_view path)
{
    std::wstring wPath(path);
    std::filesystem::path fsPath(wPath);
    std::wstring extension = fsPath.extension().wstring();

    if (extension == L".dds" || extension == L".DDS") 
        HR_T(::CreateDDSTextureFromFile(device.Get(), fsPath.c_str(), nullptr, m_ShaderResourceView.GetAddressOf()));
    else 
        HR_T(::CreateWICTextureFromFile(device.Get(), fsPath.c_str(), nullptr, m_ShaderResourceView.GetAddressOf()));
}
