#pragma once
#include "Object.h"
#include "Transform.h"

#include <algorithm>

class IUpdate;
class ILateUpdate;
class IFixedUpdate;
class IRender;

// temp
class IPreRender;

class Component;
class Transform;
class Renderer;

class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
protected:
	GameObject(std::wstring_view name);
public:
	~GameObject() override = default;

	void InitializeComponents();
	void Start();

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void FixedUpdate(float deltaTime);

	void Render(Renderer* renderer);
	void GUI();

	// temp
	void PreRender(Renderer* renderer);

	template <typename T, typename... Args>
	std::weak_ptr<T> AddComponent(Args&&... args);

	template <typename T>
	std::weak_ptr<T> GetComponent();
	template <typename T>
	std::vector<std::weak_ptr<T>> GetComponents();

	template <typename T>
	std::weak_ptr<T> GetComponentInChildren();
	template <typename T>
	std::weak_ptr<T> GetComponentInParent();

	template <typename T>
	std::vector<std::weak_ptr<T>> GetComponentsInParent();
	template <typename T>
	std::vector<std::weak_ptr<T>> GetComponentsInChildren();

	// GameObject 생성 전역 함수(Add Transform Component)
	static std::shared_ptr<GameObject> Create(std::wstring_view name);
	static std::shared_ptr<GameObject> Create(std::wstring_view name, std::weak_ptr<Transform> parent);

	// deleted constructor , = operator
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

public:
	std::shared_ptr<Transform> transform;
	std::vector<std::shared_ptr<GameObject>> m_Children;

private:
	std::vector<std::shared_ptr<Component>> m_Components;

	std::vector<std::weak_ptr<IUpdate>> m_Updates;
	std::vector<std::weak_ptr<ILateUpdate>> m_LateUpdates;
	std::vector<std::weak_ptr<IFixedUpdate>> m_FixedUpdates;
	std::vector<std::weak_ptr<IRender>> m_Renders;

	// temp
	std::vector<std::weak_ptr<IPreRender>> m_PreRenders;
	bool m_bInspector = false;
};

template <typename T, typename... Args>
std::weak_ptr<T> GameObject::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of_v<Component, T>, "T must be a derived class of Component");

	std::shared_ptr<T> component = std::make_shared<T>(weak_from_this(), std::forward<Args>(args)...);

	if constexpr (std::is_base_of_v<IUpdate, T>)
		m_Updates.emplace_back(component);
	if constexpr (std::is_base_of_v<IFixedUpdate, T>)
		m_FixedUpdates.emplace_back(component);
	if constexpr (std::is_base_of_v<ILateUpdate, T>)
		m_LateUpdates.emplace_back(component);
	if constexpr (std::is_base_of_v<IRender, T>)
		m_Renders.emplace_back(component);

	// temp
	if constexpr (std::is_base_of_v<IPreRender, T>)
		m_PreRenders.emplace_back(component);

	m_Components.push_back(component);

	return std::weak_ptr<T>(component);
}

template <typename T>
std::weak_ptr<T> GameObject::GetComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must be a derived class of Component");

	for (const auto& component : m_Components)
	{
		if (auto derivedPtr = std::dynamic_pointer_cast<T>(component))
		{
			return std::weak_ptr<T>(derivedPtr);
		}
	}

	return std::weak_ptr<T>(std::shared_ptr<T>(nullptr));
}

template <typename T>
std::vector<std::weak_ptr<T>> GameObject::GetComponents()
{
	static_assert(std::is_base_of_v<Component, T>, "T must be a derived class of Component");

	std::vector<std::weak_ptr<T>> searchedComponents;

	std::for_each(m_Components.begin(), m_Components.end(), [&searchedComponents](auto& ptr)
		{
			if(auto shared = std::dynamic_pointer_cast<T>(ptr))
			{
				searchedComponents.emplace_back(shared);
			}
		});

	return searchedComponents;
}

template <typename T>
std::weak_ptr<T> GameObject::GetComponentInChildren()
{
	static_assert(std::is_base_of_v<Component, T>, "T must be a derived class of Component");

	auto childrenTransform = GetComponent<Transform>().lock()->GetChildren();
	std::vector<std::shared_ptr<GameObject>> childrenObject;
	std::for_each(childrenTransform.begin(), childrenTransform.end(), [&childrenObject](auto& transform)
		{
			childrenObject.push_back(transform.lock()->GetOwner());
		});

	// 자식 오브젝트들을 순회하면서 컴포넌트 검색
	for (auto& child : childrenObject) 
	{
		auto component = child->GetComponent<T>();
		if (!component.expired()) {
			return component;
		}

		// 재귀적으로 자식의 자식 검색
		auto childComponent = child->GetComponentInChildren<T>();
		if (!childComponent.expired()) {
			return childComponent;
		}
	}

	return std::weak_ptr<T>(std::shared_ptr<T>(nullptr));
}

template <typename T>
std::weak_ptr<T> GameObject::GetComponentInParent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must be a derived class of Component");

	auto parent = GetComponent<Transform>().lock()->GetParent().lock();
	std::vector<std::shared_ptr<GameObject>> childrenObject;

	while (parent) {
		auto component = parent->GetOwner()->GetComponent<T>();
		if (!component.expired()) {
			return component;
		}

		parent = parent->GetParent().lock();
	}

	return std::weak_ptr<T>(std::shared_ptr<T>(nullptr));
}

template <typename T>
std::vector<std::weak_ptr<T>> GameObject::GetComponentsInParent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must be a derived class of Component");

	auto parent = GetComponent<Transform>().lock()->GetParent().lock();
	std::vector<std::shared_ptr<GameObject>> childrenObject;

	std::vector<std::weak_ptr<T>> components;

	while (parent) {
		auto parentComponents = parent->GetOwner()->GetComponents<T>();
		components.insert(components.end(), parentComponents.begin(), parentComponents.end());

		parent = parent->GetParent().lock();
	}

	return components;
}

template <typename T>
std::vector<std::weak_ptr<T>> GameObject::GetComponentsInChildren()
{
	static_assert(std::is_base_of_v<Component, T>, "T must be a derived class of Component");

	std::vector<std::weak_ptr<T>> components;

	auto childrenTransform = GetComponent<Transform>().lock()->GetChildren();
	std::vector<std::shared_ptr<GameObject>> childrenObject;
	std::for_each(childrenTransform.begin(), childrenTransform.end(), [&childrenObject](auto& transform)
		{
			childrenObject.push_back(transform->GetOwner());
		});

	for (auto& child : childrenObject) {
		auto childComponents = child->GetComponents<T>();
		components.insert(components.end(), childComponents.begin(), childComponents.end());

		auto childChildComponents = child->GetComponentsInChildren<T>();
		components.insert(components.end(), childChildComponents.begin(), childChildComponents.end());
	}

	return components;
}
