#include "pch.h"
#include "Camera.h"

#include <imgui.h>

#include "Transform.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/PerFrameCB.h"

Camera::Camera(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	Component::Initialize();
}

void Camera::LateUpdate(float deltaTime)
{
	auto transform = m_Transform.lock();

	if (!transform)
	{
		LOG_ERROR(L"nullptr : Transform");
		return;
	}

	//if(transform->IsChanged())
	UpdateMatrix(transform);
}

void Camera::UpdateMatrix(std::shared_ptr<Transform>& transform)
{
	Vector3 eye = transform->GetPosition();
	Vector3 to = { 0.0f, 0.0f, 1.0f };
	Vector3 up = transform->GetUp();
	m_MatView = XMMatrixLookToLH(eye, to, up);
	m_MatProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.f / 600.f, 1.f, 2000.f);
}

void Camera::PreRender(Renderer* renderer)
{
	auto transform = m_Transform.lock();
	auto& camPos = transform->GetPosition();


	if (!transform)
	{
		LOG_ERROR(L"nullptr : Transform");
		return;
	}

	if (transform->IsChanged())
	{
		CameraData cd
		{
			m_MatView.Transpose(),
			m_MatProjection.Transpose(),
			{camPos.x, camPos.y, camPos.z, 1.f}, // todo : transform position
		};

		FrameSettings frameSet
		{
			&cd,
			nullptr,
		};

		renderer->SetPerFrame(frameSet);
	}
}

void Camera::GUI()
{
	auto camPos = m_Transform.lock()->GetPosition();

	if (ImGui::DragFloat3("Camera Position", reinterpret_cast<float*>(&camPos), 0.5f))
		m_Transform.lock()->SetPosition(camPos);
}
