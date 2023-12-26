#include "pch.h"
#include "Texture.h"

#include <filesystem>

Texture::Texture(ComPtr<ID3D11Device> device, std::wstring_view path, btdTextureType type)
	: type(type)
{
	Create(device, path);
}

Texture::Texture(ComPtr<ID3D11ShaderResourceView> shaderResourceView, btdTextureType type)
	: type(type), m_ShaderResourceView(shaderResourceView)
{
}

void Texture::Create(ComPtr<ID3D11Device> device, std::wstring_view path)
{
    std::filesystem::path fsPath(path);
    std::wstring extension = fsPath.extension();

    name = fsPath.parent_path().parent_path().filename().wstring()
		 + L'/' + fsPath.filename().replace_extension().wstring();

    if (extension == L".dds" || extension == L".DDS")
    {
	    HR_T(::CreateDDSTextureFromFile(device.Get(), fsPath.c_str(), nullptr, m_ShaderResourceView.GetAddressOf()));
    }
    else if (extension == L".tga" || extension == L".TGA")
    {
        ScratchImage scratchImage;
        HR_T(::LoadFromTGAFile(fsPath.c_str(), nullptr, scratchImage));

        ID3D11Resource* tex = nullptr;
        auto hr = DirectX::CreateTexture(device.Get(), scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), &tex);

        if (SUCCEEDED(hr))
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = scratchImage.GetMetadata().format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = -1;

            hr = device->CreateShaderResourceView(tex, &srvDesc, m_ShaderResourceView.GetAddressOf());
            tex->Release();
        }
    }
    else
    {
        HR_T(::CreateWICTextureFromFile(device.Get(), fsPath.c_str(), nullptr, m_ShaderResourceView.GetAddressOf()));
    }
}
