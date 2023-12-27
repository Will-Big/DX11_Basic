#include "pch.h"
#include "ModelLoader.h"

#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"
#include "AnimatorController.h"
#include "Animator.h"
#include "AnimationClip.h"

ModelLoader::ModelLoader(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ModelData& modelData, std::array<std::shared_ptr<Shader>, btdShaderScope_END> shaders, std::shared_ptr<InputLayout> layout)
	: m_Device(device), m_DeviceContext(deviceContext), m_RootData(modelData), m_Shaders(shaders), m_InputLayout(layout)
{
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::ProcessAnimation(const aiScene* scene)
{
	if (scene->mNumAnimations <= 0)
		return;

	// Animator Controller 중복 검사
	if (ResourceManager::instance->animatorControllers.find(m_folderPath.wstring()) != ResourceManager::instance->animatorControllers.end())
	{
		LOG_ERROR(L"Animator Controller : Duplicated exist");
		return;
	}

	// todo : m_folderPath 로 animationController 를 생성하면 재활용이 불가능하므로 변경
	ResourceManager::instance->animatorControllers[m_folderPath.wstring()] = std::make_shared<AnimatorController>(m_folderPath.wstring());
	auto& ctrl = ResourceManager::instance->animatorControllers[m_folderPath.wstring()];

	for (uint32_t i = 0; i < scene->mNumAnimations; i++)
	{
		std::wstring wsAnimName = aiString2wstring(scene->mAnimations[i]->mName);

		auto it = std::find_if(ctrl->animationClips.begin(), ctrl->animationClips.end(), [&wsAnimName](AnimationClip& clip)
			{
				return clip.GetName() == wsAnimName;
			});

		// AnimationClip 중복 검사
		if (it != ctrl->animationClips.end())
		{
			LOG_ERROR(L"AnimationClip : Duplicated exist");
			return;
		}
		auto& clip = ctrl->animationClips.emplace_back(wsAnimName);
		clip.duration = scene->mAnimations[i]->mDuration;
		clip.framePerSecond = scene->mAnimations[i]->mTicksPerSecond;
		clip.isLoop = true;	// todo : 삭제

		// Key Frame 생성
		for (uint32_t j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
		{
			auto nodeAnim = scene->mAnimations[i]->mChannels[j];
			std::string nodeName(nodeAnim->mNodeName.C_Str());
			std::wstring wsNodeName{ nodeName.begin(), nodeName.end() };

			KeyFrames frame;
			frame.name = wsNodeName;
			LOG_MESSAGE(wsNodeName.c_str());

			// scale
			frame.scaleKeys.reserve(nodeAnim->mNumScalingKeys);
			for (uint32_t k = 0; k < nodeAnim->mNumScalingKeys; k++)
			{
				auto sk = nodeAnim->mScalingKeys[k];
				frame.scaleKeys.emplace_back(sk.mTime, sk.mValue.x, sk.mValue.y, sk.mValue.z);
			}

			// rotation
			frame.rotationKeys.reserve(nodeAnim->mNumRotationKeys);
			for (uint32_t k = 0; k < nodeAnim->mNumRotationKeys; k++)
			{
				auto rk = nodeAnim->mRotationKeys[k];
				frame.rotationKeys.emplace_back(rk.mTime, rk.mValue.x, rk.mValue.y, rk.mValue.z, rk.mValue.w);
			}

			// translation
			frame.positionKeys.reserve(nodeAnim->mNumRotationKeys);
			for (uint32_t k = 0; k < nodeAnim->mNumPositionKeys; k++)
			{
				auto pk = nodeAnim->mPositionKeys[k];
				frame.positionKeys.emplace_back(pk.mTime, pk.mValue.x, pk.mValue.y, pk.mValue.z);
			}

			clip.keyFrames.emplace_back(frame);
		}
	}
}

btdTextureType ModelLoader::aiType2btdType(aiTextureType type)
{
	switch (type)
	{
	case aiTextureType_DIFFUSE:
		return btdTextureType_DIFFUSE;
	case aiTextureType_NORMALS:
		return btdTextureType_NORMALS;
	case aiTextureType_SPECULAR:
		return btdTextureType_SPECULAR;
	case aiTextureType_OPACITY:
		return btdTextureType_OPACITY;
	case aiTextureType_EMISSIVE:
		return btdTextureType_EMISSIVE;
	case aiTextureType_METALNESS:
		return btdTextureType_METALNESS;
	case aiTextureType_SHININESS:
		return btdTextureType_ROUGHNESS;
	default:
		LOG_ERROR(L"Invalid Texture Type");
		return btdTextureType_END;
	}
}

std::wstring ModelLoader::aiString2wstring(const aiString& string)
{
	auto str = std::string(string.C_Str());
	return std::wstring(str.begin(), str.end());
}

void ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene,
                                       std::weak_ptr<Material>& material)
{
	std::wstring materialName = aiString2wstring(mat->GetName());
	auto matIt = ResourceManager::instance->materials.find(materialName);

	// 새로운 material 일 경우
	if (matIt == ResourceManager::instance->materials.end())
	{
		// 새로운 material 생성
		auto sharedMaterial = std::make_shared<Material>(materialName);
		material = sharedMaterial;

		// 리소스 매니저에 할당
		ResourceManager::instance->materials.insert({ materialName, sharedMaterial });
		matIt = ResourceManager::instance->materials.find(materialName);

		// 셰이더 및 인풋 레이아웃 참조
		material.lock()->shaders = m_Shaders;
		material.lock()->inputLayout = m_InputLayout;
	}
	material = matIt->second;

	for (UINT i = 0; i < mat->GetTextureCount(type); i++) 
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// aiString 변환
		fs::path path = m_folderPath / str.C_Str();
		fs::path key = m_folderPath.filename().wstring() + L'/'
			+ path.filename().replace_extension().wstring();
		std::wstring materialName = aiString2wstring(mat->GetName());

		btdTextureType btdType = aiType2btdType(type);

		// key 검색
		auto texIt = ResourceManager::instance->textures.find(key);

		// 새로운 texture 일 경우
		if (texIt == ResourceManager::instance->textures.end())
		{
			// 내장 texture 확인
			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());

			// 새로운 texture 생성
			std::shared_ptr<Texture> newTexture;
			if (embeddedTexture != nullptr)
				newTexture = std::make_shared<Texture>(LoadEmbeddedTexture(embeddedTexture), btdType);
			else
				newTexture = std::make_shared<Texture>(m_Device, path.wstring(), btdType);

			// 리소스 매니저에 할당 및 iterator 갱신
			ResourceManager::instance->textures.insert({ newTexture->name, newTexture });
			texIt = ResourceManager::instance->textures.find(newTexture->name);

			LOG_MESSAGE(newTexture->name.c_str());
		}

		material.lock()->SetTexture(btdType, texIt->second);
	}
}

ID3D11ShaderResourceView* ModelLoader::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture = nullptr;

	if (embeddedTexture->mHeight != 0) {
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData{};
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ID3D11Texture2D* texture2D = nullptr;
		hr = m_Device->CreateTexture2D(&desc, &subresourceData, &texture2D);
		if (FAILED(hr))
			LOG_ERROR(L"CreateTexture2D failed!");

		hr = m_Device->CreateShaderResourceView(texture2D, nullptr, &texture);
		if (FAILED(hr))
			LOG_ERROR(L"CreateShaderResourceView failed!");

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(m_Device.Get(), m_DeviceContext.Get(), reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
	if (FAILED(hr))
		LOG_ERROR(L"Texture couldn't be created from memory!");

	return texture;
}