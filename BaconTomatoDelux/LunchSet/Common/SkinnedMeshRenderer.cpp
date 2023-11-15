#include "pch.h"
#include "SkinnedMeshRenderer.h"
#include "MeshFilter.h"

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
}

void SkinnedMeshRenderer::Render(Renderer* renderer)
{

}
