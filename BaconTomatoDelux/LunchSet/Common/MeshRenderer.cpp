#include "pch.h"
#include "MeshRenderer.h"
#include "GameObject.h"

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
	auto meshfilter = m_MeshFilter.lock();
	auto transform = m_Transform.lock();

	assert(meshfilter != nullptr && transform != nullptr);

	VSObjectData td{ transform->GetMatrix().Transpose() };

	// render sub meshes
	for (const auto& mesh : meshfilter->meshes)
	{
		ObjectSettings objSet{
			mesh->vertexBuffer,
			mesh->indexBuffer,
			mesh->textures,
			&td,
		};

		renderer->SetPerObject(objSet);
		renderer->Draw();
	}
}