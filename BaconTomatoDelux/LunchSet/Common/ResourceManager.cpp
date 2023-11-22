#include "pch.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "Material.h"
#include "AnimatorController.h"

// temp(Scene)
#include "GameProcess.h"

#include <filesystem>

void ResourceManager::Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
	m_Device = device;
	m_DeviceContext = deviceContext;
}

void ResourceManager::GetStaticModelData(std::wstring_view fileName, ModelData& data, std::weak_ptr<GameObject> gameObject)
{
	auto parent = gameObject.lock();

	if (data.mesh)
	{
		SetMesh(data.mesh, parent);
		SetMaterial(RES_MAN.materials[fileName.data()], parent);
	}

	for(auto& subModel : data.subs)
	{
		auto child = GameObject::Create(subModel.name);
		auto childTransform = child->GetComponent<Transform>().lock();
		childTransform->SetParent(gameObject);

		childTransform->SetLocalScale(subModel.scale);
		childTransform->SetRotation(subModel.rotation);
		childTransform->SetLocalPosition(subModel.position);

		GetStaticModelData(fileName, subModel, child);

		// temp(Scene)
		GameProcess::m_GameObjects.push_back(child);
	}
}

void ResourceManager::SetMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<GameObject>& gameObject)
{
	LOG_MESSAGE(gameObject->GetName().c_str());

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

	meshFilter->mesh = mesh;
}

void ResourceManager::SetMaterial(std::shared_ptr<Material> material, std::weak_ptr<GameObject> gameObject)
{
	// material 연결
	if (auto meshRenderer = gameObject.lock()->GetComponent<MeshRenderer>().lock())
		meshRenderer->m_Material = material;
	else if (auto skinnedMeshRenderer = gameObject.lock()->GetComponent<SkinnedMeshRenderer>().lock())
		skinnedMeshRenderer->m_Material = material;
	else
		LOG_ERROR(L"nullptr : MeshRenderer OR SkinnedMeshRenderer");
}

