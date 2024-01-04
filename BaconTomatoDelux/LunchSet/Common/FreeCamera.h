#pragma once
#include "Component.h"
#include "IKeyProcessor.h"
#include "IUpdate.h"
#include "IFixedUpdate.h"

/**
 * \brief	: Unity 에는 없는 기능. Unity 상에서는 여러 컴포넌트를 조합해서 만들어야 함
 *			  에디터 상의 프리 카메라 구현을 위해 만듦
 */
class FreeCamera : public Component, public IUpdate, public IKeyProcessor
{
public:
	FreeCamera(std::weak_ptr<GameObject> owner);
	~FreeCamera() override;

	void Initialize() override;
	void Start() override;
	void Update(float deltaTime) override;

	void GUI() override;
	void OnInputProcess(const InputStruct& input) override;

private:
	Vector3 m_Forward;
	Vector3 m_Right;
	Vector3 m_InputDirection;

	Vector2 m_LastMousePosition;
	Vector2 m_YawPitch;
};
