#include "pch.h"
#include "SkinnedMeshRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshFilter.h"
#include "Transform.h"

#include "../Graphics/Renderer.h"
#include "../Graphics/VertexShader.h"
#include "../Graphics/PixelShader.h"


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

	auto childrenTransform = m_Transform.lock()->GetRoot().lock()->GetOwner()->GetComponentsInChildren<Transform>();

	// todo : mesh 를 하나로 합치기 + 중위순회 (feat. Mr.Hong)
	for (auto& mesh : m_MeshFilter.lock()->meshes)
	{
		for (auto& bone : mesh->boneReferences)
		{
			for (auto& child : childrenTransform)
			{
				auto ownerName = child.lock()->GetOwner()->GetName();
				std::string strName{ ownerName.begin(), ownerName.end() };

				if (bone.name == strName)
				{
					m_Bones.push_back(child.lock());
					break;
				}
			}
		}
	}
}

void SkinnedMeshRenderer::Render(Renderer* renderer)
{
	CHECK_ENABLED;

	auto meshfilter = m_MeshFilter.lock();
	auto transform = m_Transform.lock();

	assert(meshfilter != nullptr && transform != nullptr);

	std::shared_ptr<VSObjectData> od = std::make_shared<VSObjectData>();
	od->World = transform->GetMatrix().Transpose();

	for (size_t i = 0; i < meshfilter->meshes.size(); i++)
	{
		std::shared_ptr<VsMatrixPallete> vmp = std::make_shared<VsMatrixPallete>();
		for (size_t j = 0; j < m_Bones.size(); ++j)
		{
			vmp->Array[j] = (meshfilter->meshes[i]->boneReferences[j].offset
				* m_Bones[j]->GetMatrix()).Transpose();
		}

		SkinnedRenderQueueSettings settings{
			materials[i]->inputLayout,
			std::static_pointer_cast<VertexShader>(materials[i]->shaders[btdShaderScope_VERTEX]),
			std::static_pointer_cast<PixelShader>(materials[i]->shaders[btdShaderScope_PIXEL]),
			meshfilter->meshes[i]->vertexBuffer,
			meshfilter->meshes[i]->indexBuffer,
			&materials[i]->textures,
			vmp,
			od,
		};

		renderer->AddRenderQueue(settings);
	}
}

void SkinnedMeshRenderer::GUI()
{
	if (ImGui::CollapsingHeader("Skinned Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ENABLE_CHECKBOX;
	}
}
