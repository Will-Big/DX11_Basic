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


GameObject::GameObject(std::wstring_view name)
	: Object(name)
{
}

GameObject::~GameObject()
{
	auto transform = GetComponent<Transform>().lock();

	if(transform == nullptr)
		LOG_ERROR(L"nullptr : Transform");

	// todo : 객체의 삭제 명령을 받았을 때 자신이 갖고 있는 하위 객체를 모두 삭제해야 함
	// temp Scene
	for(auto& child : transform->GetChildren())
	{
		if (child.expired())
			continue;

		GameProcess::m_GameObjects.erase(std::remove_if(GameProcess::m_GameObjects.begin(), GameProcess::m_GameObjects.end(),
			[&](const std::shared_ptr<GameObject>& go)
			{
				return child.lock()->GetRoot().lock() == transform;
			}),
			GameProcess::m_GameObjects.end());
	}
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
		if(cp->IsRenderGUI())
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

	// GameObject 의 생성자가 아니라 Create 함수에서 AddComponent 를 해주는 이유
	//		생성자에서 AddComponent 를 하면, GameObject 의 생성이 완료되지 않은 시점에
	//		AddComponent 함수가 수행되면서 컴포넌트를 생성하기 위한 인자 중 GameObject 의 weak_ptr 이
	//		생성되지 않은 상황에서 AddComponent 가 이루어지기 때문에 nullptr 로 들어가게 됨
	auto transform = gameObject->AddComponent<Transform>();
	transform.lock()->SetRoot(transform);

	return gameObject;
}
