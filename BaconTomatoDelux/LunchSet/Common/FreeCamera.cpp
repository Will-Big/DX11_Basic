#include "pch.h"
#include "FreeCamera.h"
#include "InputManager.h"
#include "Transform.h"

FreeCamera::FreeCamera(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

FreeCamera::~FreeCamera()
{
	InputManager::instance->RemoveInputProcessor(this);
}

void FreeCamera::Initialize()
{
	Component::Initialize();

	InputManager::instance->AddInputProcessor(this);
}

void FreeCamera::Start()
{
	Component::Start();
}

void FreeCamera::Update(float deltaTime)
{
	auto transform = m_Transform.lock();

	// Forward와 Right는 매 프레임 업데이트 필요
	m_Forward = transform->GetForward();
	m_Right = transform->GetRight();

	if (m_InputDirection.LengthSquared() > 0)
	{
		Vector3 movement = m_InputDirection * MOVEMENT_GAIN;
		Vector3 pos = transform->GetLocalPosition();
		transform->SetLocalPosition(pos + movement);
	}

	// 회전 적용
	if (m_YawPitch.LengthSquared() > 0)
	{
		Vector3 currentRotation = transform->GetLocalEulerRotation();
		currentRotation.x += m_YawPitch.y * ROTATION_GAIN;
		currentRotation.y += m_YawPitch.x * ROTATION_GAIN;
		transform->SetLocalEulerRotation(currentRotation);
	}

	// 마우스 회전 값 초기화
	m_YawPitch = Vector2{ 0.f, 0.f };
}

void FreeCamera::OnInputProcess(const InputStruct& input)
{
	m_InputDirection = Vector3{ 0.f, 0.f, 0.f };

	if (input.keyState.IsKeyDown(Keyboard::W))
	{
		m_InputDirection += m_Forward;
	}
	else if (input.keyState.IsKeyDown(Keyboard::S))
	{
		m_InputDirection -= m_Forward;
	}

	if (input.keyState.IsKeyDown(Keyboard::A))
	{
		m_InputDirection -= m_Right;
	}
	else if (input.keyState.IsKeyDown(Keyboard::D))
	{
		m_InputDirection += m_Right;
	}

	if (m_InputDirection.LengthSquared() > 0)
	{
		m_InputDirection.Normalize();
	}


	// 마우스 회전 처리
	if (input.mouseState.rightButton)
	{
		if (input.mouseTracker.rightButton == input.mouseTracker.HELD)
		{
			// Calculate the delta between the initial and current mouse positions
			float deltaX = static_cast<float>(input.mouseState.x) - m_LastMousePosition.x;
			float deltaY = static_cast<float>(input.mouseState.y) - m_LastMousePosition.y;

			// Yaw 회전 (around Y-axis)
			m_YawPitch.x += deltaX;

			// Pitch 회전 (around X-axis)
			m_YawPitch.y += deltaY;
		}

		m_LastMousePosition.x = static_cast<float>(input.mouseState.x);
		m_LastMousePosition.y = static_cast<float>(input.mouseState.y);
	}
}
