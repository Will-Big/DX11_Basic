#include "pch.h"
#include "Transform.h"
#include "GameObject.h"

Transform::Transform(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

Transform::~Transform()
{
}

void Transform::Update(float deltaTime)
{
	UpdateTransform();
}

void Transform::LookAt(Vector3&& position)
{
	// todo
}

void Transform::LookAt(std::weak_ptr<GameObject> position)
{
	// todo
}

void Transform::SetScale(const Vector3& scale)
{
	if (auto parent = m_Parent.lock())
	{
		Vector3 parentScale = parent->GetScale();
		Vector3 scale = m_Scale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(m_Scale);
	}
}

void Transform::SetEulerRotation(const Vector3& localEulerRotation)
{
}

void Transform::SetRotation(const Quaternion& localRotation)
{
}

void Transform::SetPosition(const Vector3& position)
{
	if (auto parent = m_Parent.lock())
	{
		Matrix worldToParentLocalMatrix = parent->GetMatrix().Invert();

		Vector3 localPosition;
		localPosition.Transform(position, worldToParentLocalMatrix);

		SetLocalPosition(localPosition);
	}
	else
	{
		SetLocalPosition(position);
	}

	m_Position = position;
}

void Transform::SetParent(std::weak_ptr<GameObject> parent)
{
	const auto parentTransform = parent.lock()->GetComponent<Transform>();
	m_Parent = parentTransform;

	std::weak_ptr<Transform> root;
	root = m_Parent;

	while(!root.expired())
	{
		if (!root.lock()->GetParent().expired())
			root = root.lock()->GetParent();
		else
			break;
	}

	m_Root = root;
	m_Parent.lock()->m_Children.push_back(m_Owner.lock()->GetComponent<Transform>());
}

void Transform::UpdateTransform()
{
	// 현 프레임에 변화 없음
	if (!m_bDirty)
	{
		m_bDirtyFrame = false;
		return;
	}

	// 현 프레임에 변화 있음
	m_bDirty = false;
	m_bDirtyFrame = true;

	m_LocalMatrix = 
		Matrix::CreateScale(m_LocalScale) * 
		Matrix::CreateFromQuaternion(m_LocalRotation) * 
		Matrix::CreateTranslation(m_LocalPosition);

	if (auto parentTransform = m_Parent.lock())
	{
		m_Matrix = m_LocalMatrix * parentTransform->m_Matrix;
	}
	else
	{
		m_Matrix = m_LocalMatrix;
	}

	for (auto& child : m_Children)
	{
		if (auto sharedChild = child.lock())
		{
			sharedChild->m_bDirty = true;
			sharedChild->UpdateTransform();
		}
		else
			LOG_ERROR(L"nullptr : child Transform");
	}
}

