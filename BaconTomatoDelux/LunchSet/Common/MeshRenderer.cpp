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

#include "../Graphics/VertexShader.h"
#include "../Graphics/PixelShader.h"

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
	//assert(meshfilter != nullptr && transform != nullptr && !materials.empty()); // 임시 주석(나중에 이걸로 사용할 예정)
	// info : 유니티에서는 유연성을 위하여 nullptr 이더라도 실행 중 넘기게 만들었음

	VSObjectData td{ transform->GetMatrix().Transpose() };

	// render sub meshes
	for (size_t i = 0; i < meshfilter->meshes.size(); i++)
	{
		// old
		ObjectSettings objSet2{
			meshfilter->meshes[i]->vertexBuffer,
			meshfilter->meshes[i]->indexBuffer,
			nullptr,
			&td,
		};

		if(materials[i] != nullptr)
		{
			objSet2.textures = &materials[i]->textures;
		}

		renderer->SetPerObject(objSet2);
		renderer->Draw();

		// new
		// 이렇게 하나하나 박는것보다 머터리얼을 통째로 넘겨주는게...? 뭐가 더 이쁠까, 뭐가 더 좋을까
		//{
		//	RenderQueueSettings settings{
		//		materials[i]->inputLayout,
		//		std::static_pointer_cast<VertexShader>(materials[i]->shaders[btdShaderScope_VERTEX]),
		//		std::static_pointer_cast<PixelShader>(materials[i]->shaders[btdShaderScope_PIXEL]),
		//		meshfilter->meshes[i]->vertexBuffer,
		//		meshfilter->meshes[i]->indexBuffer,
		//		&materials[i]->textures,
		//		&td
		//	};

		//	renderer->AddRenderQueue(settings);
		//}
	}
}