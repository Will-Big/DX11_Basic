#pragma once
#include "Component.h"
#include "IFixedUpdate.h"
#include "IKeyProcessor.h"

/**
 * \brief	: Unity 에는 없는 기능. Unity 상에서는 여러 컴포넌트를 조합해서 만들어야 함
 *			  에디터 상의 프리 카메라 구현을 위해 만듦
 */
class Movement : public Component, public IFixedUpdate, public IKeyProcessor
{
public:
	Movement(std::weak_ptr<GameObject> owner);
	~Movement() override = default;

	void Initialize() override;
	void Start() override;
	void FixedUpdate(float deltaTime) override;

	void GUI() override;
	void OnInputProcess(const InputStruct& input) override;

private:
	float m_Speed = 350.0f;

	Vector3 m_Forward;
	Vector3 m_Right;
	Vector3 m_InputDirection;

	Vector3 m_LastMousePosition;
	Vector3 m_DeltaRotation;
};
