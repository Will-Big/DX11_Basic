#pragma once
#include "Component.h"
#include "IUpdate.h"

class GameObject;

class Transform : public Component, public IUpdate
{
public:
	Transform(std::weak_ptr<GameObject> owner);
	~Transform() override;

	// Cycle Interface
	void Update(float deltaTime) override;

	void LookAt(Vector3&& position);
	void LookAt(std::weak_ptr<GameObject> position);

	// Local
	void SetLocalScale(const Vector3& localScale) { m_LocalScale = localScale; m_bDirty = true; }
	void SetLocalEulerRotation(const Vector3& localEulerRotation) { m_LocalRotation = Quaternion::CreateFromYawPitchRoll(localEulerRotation); m_bDirty = true; }
	void SetLocalRotation(const Quaternion& localRotation) { m_LocalRotation = localRotation; m_bDirty = true; }
	void SetLocalPosition(const Vector3& localPosition) { m_LocalPosition = localPosition; m_bDirty = true; }

	// World
	void SetMatrix(const Matrix& matrix) { m_Matrix = matrix; }
	void SetScale(const Vector3& scale);
	void SetEulerRotation(const Vector3& localEulerRotation);
	void SetRotation(const Quaternion& localRotation);
	void SetPosition(const Vector3& position);

	void SetParent(std::weak_ptr<GameObject> parent);

	// Local
	const Vector3& GetLocalScale() { return m_LocalScale; }
	const Quaternion& GetLocalRotation() { return m_LocalRotation; }
	const Vector3& GetLocalEulerRotation() { return m_LocalRotation.ToEuler(); }
	const Vector3& GetLocalPosition() { return m_LocalPosition; }

	// World
	const Matrix& GetMatrix() { return m_Matrix; }
	const Vector3& GetScale() { return m_Scale; }
	const Quaternion& GetRotation() { return m_Rotation; }
	const Vector3& GetEulerRotation() { return m_Rotation.ToEuler(); }
	const Vector3& GetPosition() { return m_Position; }

	Vector3 GetRight() { return m_Matrix.Right(); }
	Vector3 GetForward() { return m_Matrix.Backward(); }
	Vector3 GetUp() { return m_Matrix.Up(); }

	std::weak_ptr<Transform> GetRoot() { return m_Root; }
	std::weak_ptr<Transform> GetParent() { return m_Parent; }
	std::vector<std::weak_ptr<Transform>>& GetChildren() { return m_Children; }

	bool IsChanged() { return m_bDirtyFrame; }

private:
	void UpdateTransform();

private:
	// Local
	Vector3 m_LocalScale = { 1.f, 1.f, 1.f };
	Quaternion m_LocalRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	Vector3 m_LocalPosition = { 0.f, 0.f, 0.f };
	Matrix m_LocalMatrix = Matrix::Identity;

	// World
	Vector3 m_Scale = { 1.f, 1.f, 1.f };
	Quaternion m_Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	Vector3 m_Position = { 0.f, 0.f, 0.f };
	Matrix m_Matrix = Matrix::Identity;

	// Hierarchy
	std::weak_ptr<Transform> m_Root;
	std::weak_ptr<Transform> m_Parent;
	std::vector<std::weak_ptr<Transform>> m_Children;

	// Changed Flags
	bool m_bDirty = true;			// for Transform Component
	bool m_bDirtyFrame = false;		// for Other Components
};
