﻿#include "pch.h"
#include "MeshRenderer.h"

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
	auto meshfilter = m_MeshFilter.lock();
	auto transform = m_Transform.lock();

	if (!meshfilter or !transform)
	{
		LOG_MESSAGE(L"nullptr Component");
		return;
	}

	TransformData td {transform->GetMatrix().Transpose()};

	ObjectSettings objSet {
		meshfilter->mesh->vertexBuffer,
		meshfilter->mesh->indexBuffer,
		meshfilter->mesh->textures,
		&td,
	};

	renderer->SetPerObject(objSet);
	renderer->Draw();
}