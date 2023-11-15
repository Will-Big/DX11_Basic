#pragma once
#include "ILateUpdate.h"
#include "Object.h"

#include <algorithm>

class IUpdate;
class ILateUpdate;
class IFixedUpdate;
class IRender;

// temp
class IPreRender;

class Component;
class Renderer;

class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
protected:
	GameObject(std::wstring_view name);
public:
	~GameObject() override;

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
	T* GetComponentInParent();

	// GameObject 생성 전역 함수(Add Transform Component)
	static std::shared_ptr<GameObject> Create(std::wstring_view name);

	// deleted constructor & operator
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

private:
	std::vector<std::shared_ptr<Component>> m_Components;

	std::vector<std::weak_ptr<IUpdate>> m_Updates;
	std::vector<std::weak_ptr<ILateUpdate>> m_LateUpdates;
	std::vector<std::weak_ptr<IFixedUpdate>> m_FixedUpdates;
	std::vector<std::weak_ptr<IRender>> m_Renders;

	// temp
	std::vector<std::weak_ptr<IPreRender>> m_PreRenders;
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
T* GameObject::GetComponentInParent()
{
}
