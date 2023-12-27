#include "pch.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "Material.h"

// temp(Scene)
#include "GameProcess.h"

ResourceManager* ResourceManager::instance = new ResourceManager{};

void ResourceManager::Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
	m_Device = device;
	m_DeviceContext = deviceContext;
}

void ResourceManager::Finalize()
{
	delete instance;
}

void ResourceManager::GetModel(std::wstring_view resourceName, std::weak_ptr<GameObject> gameObject)
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


std::shared_ptr<GameObject> ResourceManager::GetModel(std::wstring_view resourceName, std::wstring& objectName)
{
	if (const auto searched = models.find(resourceName.data()); searched == models.end())
	{
		MessageBoxW(nullptr, L"Data needs to be loaded before it can be accessed", L"LOG_ERROR", MB_OK);
		return nullptr;
	}
	else
	{
		std::shared_ptr<GameObject> go = GameObject::Create(objectName);
		LinkModelData(resourceName, searched->second, std::weak_ptr(go));

		return go;
	}
}


void ResourceManager::LinkModelData(std::wstring_view fileName, ModelData& data, std::weak_ptr<GameObject> gameObject)
{
	auto object = gameObject.lock();

	if (!data.meshes.empty())
	{
		for(size_t i = 0; i < data.meshes.size(); i++)
		{
			SetMesh(data.meshes[i], object);
			SetMaterial(data.materials[i].lock(), object);
		}
	}

	for(auto& subModel : data.children)
	{
		auto child = GameObject::Create(subModel.name);
		auto childTransform = child->GetComponent<Transform>().lock();
		childTransform->SetParent(gameObject);

		childTransform->SetLocalScale(subModel.scale);
		childTransform->SetLocalRotation(subModel.rotation);
		childTransform->SetLocalPosition(subModel.position);

		LinkModelData(fileName, subModel, child);

		// temp(Scene)
		GameProcess::m_GameObjects.push_back(child);
	}
}

void ResourceManager::SetMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<GameObject>& gameObject)
{
	// 필수 컴포넌트 생성 및 연결
	if(mesh->boneReferences.empty()) // MeshRenderer (Static Mesh)
	{
		auto meshRenderer = gameObject->GetComponent<MeshRenderer>().lock();
		if (meshRenderer == nullptr)
			meshRenderer = gameObject->AddComponent<MeshRenderer>().lock();
	}
	else // SkinnedMeshRenderer (Skeletal Mesh)
	{
		auto skinnedMeshRenderer = gameObject->GetComponent<SkinnedMeshRenderer>().lock();
		if (skinnedMeshRenderer == nullptr)
			skinnedMeshRenderer = gameObject->AddComponent<SkinnedMeshRenderer>().lock();
	}

	auto meshFilter = gameObject->GetComponent<MeshFilter>().lock();
	if (meshFilter == nullptr)
		meshFilter = gameObject->AddComponent<MeshFilter>().lock();

	meshFilter->meshes.push_back(mesh);
}

void ResourceManager::SetMaterial(std::shared_ptr<Material> material, std::weak_ptr<GameObject> gameObject)
{
	// material 연결
	if (auto meshRenderer = gameObject.lock()->GetComponent<MeshRenderer>().lock())
		meshRenderer->materials.push_back(material);
	else if (auto skinnedMeshRenderer = gameObject.lock()->GetComponent<SkinnedMeshRenderer>().lock())
		skinnedMeshRenderer->m_Material = material;	// todo : 위와 같이 변경
	else
		LOG_ERROR(L"nullptr : MeshRenderer OR SkinnedMeshRenderer");
}