#include "pch.h"
#include "GameObject.h"

#include "Transform.h"

#include "IUpdate.h"
#include "ILateUpdate.h"
#include "IFixedUpdate.h"
#include "IPreRender.h"
#include "IRender.h"
#include "../Graphics/Renderer.h"


GameObject::GameObject(std::wstring_view name)
	: Object(name)
{
}

GameObject::~GameObject()
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
}

void GameObject::Start()
{
	for (auto& cp : m_Components)
	{
		if (cp == nullptr)
			continue;

		cp->Start();
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
}

void GameObject::GUI()
{
	for (auto& cp : m_Components)
	{
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
	gameObject->AddComponent<Transform>();

	return gameObject;
}
