#include "pch.h"
#include "SkinnedMeshRenderer.h"

#include "Mesh.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "Transform.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/PerObjectCB.h"


SkinnedMeshRenderer::SkinnedMeshRenderer(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
}

void SkinnedMeshRenderer::Initialize()
{
	Component::Initialize();
	m_MeshFilter = m_Owner.lock()->GetComponent<MeshFilter>();

	auto childrenTransform = m_Transform.lock()->GetRoot().lock()->GetOwner().lock()->GetComponentsInChildren<Transform>();

	// bone mapping
	for(auto& boneRef : m_MeshFilter.lock()->mesh->boneReferences)
	{
		for(auto& wChild : childrenTransform)
		{
			auto sChild = wChild.lock();
			std::string sName(sChild->GetOwner().lock()->GetName().begin(), sChild->GetOwner().lock()->GetName().end());
			if(boneRef.name == sName)
			{
				m_Bones.push_back(sChild);
				break;
			}
		}
	}
}

void SkinnedMeshRenderer::Render(Renderer* renderer)
{
	auto meshfilter = m_MeshFilter.lock();
	auto transform = m_Transform.lock();

	assert(meshfilter != nullptr && transform != nullptr);

	VSObjectData td{ transform->GetMatrix().Transpose() };

	ObjectSettings objSet{
		meshfilter->mesh->vertexBuffer,
		meshfilter->mesh->indexBuffer,
		meshfilter->mesh->textures,
		&td,
	};
	renderer->SetPerObject(objSet);

	VsMatrixPallete mp;
	for(size_t i = 0; i < m_Bones.size(); ++i)
	{
		mp.Array[i] = (meshfilter->mesh->boneReferences[i].offset * m_Bones[i]->GetMatrix()).Transpose();
	}

	MatrixPalleteSettings mpSet{
		&mp
	};

	renderer->SetMatrixPallete(mpSet);


	renderer->Draw();
}
