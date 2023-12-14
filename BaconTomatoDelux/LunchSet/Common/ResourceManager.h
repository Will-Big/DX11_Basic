#pragma once
#include "ModelLoader.h"
#include "Mesh.h"

#include "../Graphics/Shader.h"
#include "../Graphics/InputLayout.h"

#include <filesystem>

namespace fs = std::filesystem;

#define RES_MAN ResourceManager::GetInstance()

class Sampler;
class InputLayout;
class Material;
class AnimatorController;

class ResourceManager
{
	// Singleton Manager
public:
	void Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	template<typename T>
	void LoadShader(std::string_view fileName, std::string_view entryPoint, D3D_SHADER_MACRO* macro, std::wstring_view resourceName);

	template<typename T, typename = decltype(T::desc)>
	void LoadModel(std::wstring_view modelFolderName, std::wstring_view resourceName);

	template <typename T>
	std::shared_ptr<T> Get(std::wstring_view resourceName);

	template <typename T>
	void GetModel(std::wstring_view resourceName, std::weak_ptr<GameObject> gameObject);

	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

private:
	void LinkModelData(std::wstring_view fileName, ModelData& data, std::weak_ptr<GameObject> gameObject);
	void SetMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<GameObject>& gameObject);
	void SetMaterial(std::shared_ptr<Material> material, std::weak_ptr<GameObject> gameObject);

	ResourceManager() = default;
	~ResourceManager() = default;

public:
	// Graphics API
	std::map<std::wstring, std::shared_ptr<Shader>> shaders;
	std::map<std::wstring, std::shared_ptr<InputLayout>> inputLayouts;
	std::map<std::wstring, std::shared_ptr<Sampler>> samplers;

	// Model
	std::map<std::wstring, ModelData> models;
	std::map<std::wstring, std::shared_ptr<Material>> materials;
	std::map<std::wstring, std::shared_ptr<AnimatorController>> animatorControllers;

	// Path
	const fs::path shaderPath = fs::path(L"../Resource/Shader/");
	//const fs::path modelPath = fs::path(L"../Resource/FBX/");
	const fs::path modelPath = fs::path(L"../Resource/Model/");

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
};

template <typename T>
void ResourceManager::LoadShader(std::string_view fileName, std::string_view entryPoint, D3D_SHADER_MACRO* macro, std::wstring_view resourceName)
{
	static_assert(std::is_base_of_v<Shader, T>, "Unsupported file type");

	// 중복 검사
	if (shaders.find(resourceName.data()) != shaders.end())
		return;

	// 파일 경로 설정 및 유효성 검사
	const fs::path filePath = shaderPath / fs::path(fileName);
	if (!exists(filePath) or filePath.extension() != ".hlsl")
	{
		MessageBoxW(nullptr, L"File does not exist", L"LOG_ERROR", MB_OK);
		return;
	}

	// 객체 생성
	std::shared_ptr<Shader> shader = std::make_shared<T>(m_Device);
	shader->Create(filePath.wstring().c_str(), entryPoint.data(), macro);

	shaders.insert({ resourceName.data(), shader });
}

template <typename T, typename>
void ResourceManager::LoadModel(std::wstring_view modelFolderName, std::wstring_view resourceName)
{
	// 중복 검사
	if (models.find(resourceName.data()) != models.end())
		return;

	// 폴더 경로 설정 및 유효성 검사
	fs::path folderPath = modelPath / fs::path(modelFolderName);
	if (!fs::is_directory(folderPath)) {
		MessageBoxW(nullptr, L"Invalid folder path", L"LOG_ERROR", MB_OK);
		return;
	}

	// 객체 생성
	ModelData modelData;
	ModelLoader modelLoader{ m_Device, m_DeviceContext, modelData };
	modelLoader.Load<T>(folderPath);

	models.insert({ resourceName.data(), modelData });
}

template <typename T>
std::shared_ptr<T> ResourceManager::Get(std::wstring_view resourceName)
{
	constexpr bool isSupportedType =
		std::is_base_of_v<Shader, T> ||
		std::is_same_v<InputLayout, T> ||
		std::is_same_v<Sampler, T>;

	static_assert(isSupportedType, "Unsupported file type");

	if constexpr (std::is_base_of_v<Shader, T>)
	{
		// 데이터 유효성 검사
		if (const auto searched = shaders.find(resourceName.data()); searched != shaders.end())
			return std::dynamic_pointer_cast<T>(searched->second);
	}
	else if constexpr (std::is_same_v<InputLayout, T>)
	{
		// 데이터 유효성 검사
		if (const auto searched = inputLayouts.find(resourceName.data()); searched != inputLayouts.end())
			return searched->second;
	}
	else if constexpr (std::is_same_v<Sampler, T>)
	{
		// 데이터 유효성 검사
		if (const auto searched = samplers.find(resourceName.data()); searched != samplers.end())
			return searched->second;
	}

	MessageBoxW(nullptr, L"Data needs to be loaded before it can be accessed", L"LOG_ERROR", MB_OK);
	return nullptr;
}

template <typename T>
void ResourceManager::GetModel(std::wstring_view resourceName, std::weak_ptr<GameObject> gameObject)
{
	constexpr bool isSupportedType =
		std::is_same_v<ModelData, T>;

	static_assert(isSupportedType, "Unsupported file type");

	if constexpr (std::is_same_v<ModelData, T>)
	{
		if (const auto searched = models.find(resourceName.data()); searched == models.end())
		{
			MessageBoxW(nullptr, L"Data needs to be loaded before it can be accessed", L"LOG_ERROR", MB_OK);
		}
		else
		{
			LinkModelData(resourceName, searched->second, gameObject);
		}
	}
}
