#pragma once
#include "ModelLoader.h"
#include "Mesh.h"

#define RES_MAN ResourceManager::GetInstance()

class ModelLoader;
class Material;
class AnimatorController;

class ResourceManager
{
	// Singleton Manager
public:
	void Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	template <typename T>
	void Load(std::wstring_view fileName);

	template <typename T, typename V>
	void Load(std::wstring_view fileName);

	template <typename T>
	void Get(std::wstring_view fileName, std::weak_ptr<GameObject> gameObject);

	static ResourceManager& GetInstance() { static ResourceManager instance; return instance; }

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

private:
	void GetStaticModelData(std::wstring_view fileName, ModelData& data, std::weak_ptr<GameObject> gameObject);
	//void GetSkinnedModelData(std::wstring_view fileName, ModelData& data, std::weak_ptr<GameObject> gameObject);
	void SetMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<GameObject>& gameObject);
	void SetMaterial(std::shared_ptr<Material> material, std::weak_ptr<GameObject> gameObject);

	ResourceManager() = default;
	~ResourceManager() = default;

public:
	std::unordered_map<std::wstring, ModelData> models;
	std::unordered_map<std::wstring, std::shared_ptr<Material>> materials;
	std::unordered_map<std::wstring, std::shared_ptr<AnimatorController>> animatorControllers;

	const std::wstring modelPath = L"../Resource/FBX/";
	const std::wstring texturePath = L"../Resource/Texture/";

private:
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;

};

template <typename T>
void ResourceManager::Load(std::wstring_view fileName)
{
}

template <typename T, typename V>
void ResourceManager::Load(std::wstring_view fileName)
{
	static_assert(std::is_same_v<ModelData, T>, "The ResourceManager::Load<T, V> of T must be a ModelData class");

	// 중복 검사
	if (models.find(fileName.data()) != models.end())
		return;

	// 파일 경로 설정
	std::string fileStr{ fileName.cbegin(), fileName.cend() };
	fileStr = std::string{ modelPath.begin(), modelPath.end() } + fileStr;

	// 임시 객체 생성
	ModelData modelData;
	ModelLoader modelLoader{ m_Device, m_DeviceContext, modelData };
	modelLoader.Load<V>(fileStr);

	models.insert({ fileName.data(), modelData });
}

template <typename T>
void ResourceManager::Get(std::wstring_view fileName, std::weak_ptr<GameObject> gameObject)
{
	if constexpr (std::is_same_v<ModelData, T>)
	{
		auto founded = models.find(fileName.data());
		if (founded == models.end())
		{
			MessageBoxW(NULL, L"The model data needs to be loaded before it can be accessed", L"LOG_ERROR", MB_OK);
			return;
		}

		GetStaticModelData(fileName, founded->second, gameObject);
	}
}
