﻿#include "pch.h"
#include "Transform.h"

#include <imgui.h>

#include "GameObject.h"

Transform::Transform(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

void Transform::Initialize()
{
	Component::Initialize();
}

void Transform::Update(float deltaTime)
{
	UpdateTransform();
}

void Transform::LookAt(const Vector3& position)
{
	// todo
}

void Transform::LookAt(std::weak_ptr<GameObject> target)
{
	// todo
}

void Transform::SetLocalMatrix(const Matrix& matrix)
{

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

	while (!root.expired())
	{
		if (!root.lock()->GetParent().expired())
			root = root.lock()->GetParent();
		else
			break;
	}

	m_Root = root;
	m_Parent.lock()->m_Children.push_back(m_Owner.lock()->GetComponent<Transform>().lock());
}

void Transform::SetParent(std::weak_ptr<Transform> parent)
{
	m_Parent = parent;

	std::weak_ptr<Transform> root;
	root = m_Parent;

	while (!root.expired())
	{
		if (!root.lock()->GetParent().expired())
			root = root.lock()->GetParent();
		else
			break;
	}

	m_Root = root;
	m_Parent.lock()->m_Children.push_back(m_Owner.lock()->GetComponent<Transform>().lock());
}

void Transform::UpdateTransform()
{
	// 현 프레임에 변화 없음
	if (!m_bDirty)
	{
		m_bDirtyFrame = false;
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

	// Decompose 가 생각보다 오버헤드가 큼
	m_Matrix.Decompose(m_Scale, m_Rotation, m_Position);
}

void Transform::GUI()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Local Scale
		if (ImGui::DragFloat3("Local Scale", &m_LocalScale.x, 0.1f))
		{
			m_bDirty = true;
		}

		// Local Rotation (as Euler angles)
		Vector3 eulerRotation = m_LocalRotation.ToEuler(); // Convert quaternion to Euler if needed
		if (ImGui::DragFloat3("Local Rotation", &eulerRotation.x, 0.1f))
		{
			m_LocalRotation = Quaternion::CreateFromYawPitchRoll(eulerRotation);
			m_bDirty = true;
		}

		// Local Position
		if (ImGui::DragFloat3("Local Position", &m_LocalPosition.x, 0.1f))
		{
			m_bDirty = true;
		}
	}
}

