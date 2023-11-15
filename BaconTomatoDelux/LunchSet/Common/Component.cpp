#include "pch.h"
#include "Component.h"
#include "Transform.h"

Component::Component(std::weak_ptr<GameObject> owner)
	: m_Owner(owner)
{
}

void Component::Initialize()
{
	SetComponentName();
	m_Transform = m_Owner.lock()->GetComponent<Transform>();
}

void Component::SetComponentName()
{
	std::string sName = typeid(*this).name();
	m_Name.assign(sName.begin(), sName.end());
}
