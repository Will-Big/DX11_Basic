﻿#include "pch.h"
#include "Movement.h"
#include "InputManager.h"
#include "Transform.h"

Movement::Movement(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

void Movement::Initialize()
{
	Component::Initialize();

	InputManager::instance->AddInputProcessor(this);
}

void Movement::Start()
{
	Component::Start();
}

void Movement::FixedUpdate(float deltaTime)
{
	auto transform = m_Transform.lock();

	// Forward와 Right는 매 프레임 업데이트 필요
	m_Forward = transform->GetForward();
	m_Right = transform->GetRight();

	if (m_InputDirection.LengthSquared() > 0)
	{
		Vector3 movement = m_InputDirection * m_Speed * deltaTime;
		Vector3 pos = transform->GetLocalPosition();
		transform->SetLocalPosition(pos + movement);
	}

	// 회전 적용
	if(m_YawPitch.LengthSquared() > 0)
	{
		Vector3 currentRotation = transform->GetLocalEulerRotation();
		currentRotation.x += m_YawPitch.y * deltaTime;
		currentRotation.y += m_YawPitch.x * deltaTime;
		transform->SetLocalEulerRotation(currentRotation);
	}

	// 마우스 회전 값 초기화
	m_YawPitch = Vector2{ 0.f, 0.f};
}

void Movement::GUI()
{
	auto transform = m_Transform.lock();

	if (ImGui::Begin("Movement Control"))
	{
		// 최대 속도 조정
		ImGui::SliderFloat("Speed", &m_Speed, 0.0f, 500.0f);

		// 현재 입력 방향 표시
		ImGui::Text("Input Direction: X: %.2f, Y: %.2f, Z: %.2f", m_InputDirection.x, m_InputDirection.y, m_InputDirection.z);

		// transform의 현재 위치 표시
		Vector3 currentPosition = transform->GetLocalPosition();
		ImGui::Text("Current Position: X: %.2f, Y: %.2f, Z: %.2f", currentPosition.x, currentPosition.y, currentPosition.z);
	}
	ImGui::End();
}

void Movement::OnInputProcess(const InputStruct& input)
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
	if (input.mouseState.leftButton)
	{
		if (input.mouseTracker.leftButton == input.mouseTracker.HELD)
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
	else
	{
		m_LastMousePosition = Vector2{ 0.f, 0.f };
	}
}
