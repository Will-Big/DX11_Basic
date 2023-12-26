#include "pch.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Material.h"

#include "Mesh.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/PerObjectCB.h"

MeshRenderer::MeshRenderer(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Initialize()
{
	Component::Initialize();

	m_MeshFilter = m_Owner.lock()->GetComponent<MeshFilter>();
}

void MeshRenderer::Start()
{
}

void MeshRenderer::Render(Renderer* renderer)
{
	if(m_Owner.lock()->GetName() == L"Primrose_Pubes")
	{
		return;
	}

	auto meshfilter = m_MeshFilter.lock();
	auto transform = m_Transform.lock();

	assert(meshfilter != nullptr && transform != nullptr);

	VSObjectData td{ transform->GetMatrix().Transpose() };

	// render sub meshes
	for(size_t i = 0; i < meshfilter->meshes.size(); i++)
	{
		ObjectSettings objSet2{
			meshfilter->meshes[i]->vertexBuffer,
			meshfilter->meshes[i]->indexBuffer,
			nullptr,
			&td,
		};

		if (materials[i] != nullptr)
			objSet2.textures = &materials[i]->textures;

		renderer->SetPerObject(objSet2);
		renderer->Draw();
	}
}