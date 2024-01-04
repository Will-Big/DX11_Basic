#include "pch.h"
#include "GameObject.h"

#include "Transform.h"
#include "GameProcess.h"

#include "IUpdate.h"
#include "ILateUpdate.h"
#include "IFixedUpdate.h"
#include "IPreRender.h"
#include "IRender.h"
#include "../Graphics/Renderer.h"

#include "imgui.h"

constexpr float indent = 10.f;

GameObject::GameObject(std::wstring_view name)
	: Object(name)
{
}

void GameObject::InitializeComponents()
{
	for (auto& cp : m_Components)
	{
		if (cp == nullptr)
			continue;

		cp->Initialize();
	}

	for (auto& child : transform->GetChildren())
	{
		child->GetOwner()->InitializeComponents();
	}
}

void GameObject::Start()
{
	for (auto& cp : m_Components)
	{
		if (cp == nullptr)
			continue;

		cp->Start();
	}

	for (auto& child : transform->GetChildren())
	{
		child->GetOwner()->Start();
	}
}

void GameObject::Update(float deltaTime)
{
	for (auto& u : m_Updates)
	{
		auto su = u.lock();

		if (su == nullptr)
			continue;

		su->Update(deltaTime);
	}

	for (auto& child : transform->GetChildren())
	{
		child->GetOwner()->Update(deltaTime);
	}
}

void GameObject::LateUpdate(float deltaTime)
{
	for (auto& lu : m_LateUpdates)
	{
		auto slu = lu.lock();

		if (slu == nullptr)
			continue;

		slu->LateUpdate(deltaTime);
	}

	for (auto& child : transform->GetChildren())
	{
		child->GetOwner()->LateUpdate(deltaTime);
	}
}

void GameObject::FixedUpdate(float deltaTime)
{
	for (auto& fu : m_FixedUpdates)
	{
		auto sfu = fu.lock();

		if (sfu == nullptr)
			continue;

		sfu->FixedUpdate(deltaTime);
	}

	for (auto& child : transform->GetChildren())
	{
		child->GetOwner()->FixedUpdate(deltaTime);
	}
}

void GameObject::Render(Renderer* renderer)
{
	for (auto& r : m_Renders)
	{
		auto sr = r.lock();

		if (sr == nullptr)
			continue;

		sr->Render(renderer);
	}

	for (auto& child : transform->GetChildren())
	{
		child->GetOwner()->Render(renderer);
	}
}

void GameObject::GUI()
{
	if (ImGui::CollapsingHeader(std::string(m_Name.begin(), m_Name.end()).c_str()))
	{
		ImGui::Indent(indent);

		for (auto& child : transform->GetChildren())
		{
			child->GetOwner()->GUI();
		}

		ImGui::Unindent(indent);
	}

	for (auto& cp : m_Components)
	{
		if (cp->IsRenderGUI())
			cp->GUI();
	}
}

void GameObject::PreRender(Renderer* renderer)
{
	for (auto& r : m_PreRenders)
	{
		auto spr = r.lock();

		if (spr == nullptr)
			continue;

		spr->PreRender(renderer);
	}

	for (auto& child : transform->GetChildren())
	{
		child->GetOwner()->PreRender(renderer);
	}
}

std::shared_ptr<GameObject> GameObject::Create(std::wstring_view name)
{
	// GameObject 생성자를 숨기는 용도의 타입
	struct AccessibleGameObject : GameObject
	{
		AccessibleGameObject(std::wstring_view name) : GameObject(name) {}
		~AccessibleGameObject() override = default;
	};

	auto gameObject = std::make_shared<AccessibleGameObject>(name);
	auto transform = gameObject->AddComponent<Transform>();
	gameObject->transform = transform.lock();
	transform.lock()->SetRoot(transform);

	return gameObject;
}

std::shared_ptr<GameObject> GameObject::Create(std::wstring_view name, std::weak_ptr<Transform> parent)
{
	// GameObject 생성자를 숨기는 용도의 타입
	struct AccessibleGameObject : GameObject
	{
		AccessibleGameObject(std::wstring_view name) : GameObject(name) {}
		~AccessibleGameObject() override = default;
	};

	auto gameObject = std::make_shared<AccessibleGameObject>(name);
	auto transform = gameObject->AddComponent<Transform>();
	gameObject->transform = transform.lock();
	transform.lock()->SetParent(parent);

	return gameObject;
}
