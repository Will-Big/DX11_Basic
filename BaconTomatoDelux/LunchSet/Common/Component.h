#pragma once
#include "Object.h"

class GameObject;
class Transform;

class Component : public Object
{
public:
	Component(std::weak_ptr<GameObject> owner);
	~Component() override = default;

	virtual void Initialize();
	virtual void Start() {}
	virtual void GUI() {}

	void SetEnabled(bool enable) { m_bEnabled = enable; };
	bool IsEnabled() { return m_bEnabled; }
	void SetRenderGUI(bool bGUI) { m_bGUI = bGUI; }
	bool IsRenderGUI() { return m_bGUI; }

	std::weak_ptr<GameObject> GetOwner() { return m_Owner; }

private:
	void SetComponentName();

protected:
	std::weak_ptr<GameObject> m_Owner;
	std::weak_ptr<Transform> m_Transform;

	bool m_bEnabled = true;
	bool m_bGUI = false;
};

