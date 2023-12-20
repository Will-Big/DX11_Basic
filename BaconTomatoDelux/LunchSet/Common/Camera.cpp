#include "pch.h"
#include "Camera.h"

#include <imgui.h>

#include "Transform.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/FixedCB.h"
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

	m_MatProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1800.f / 1200.f, 1.f, 2000.f);
}

void Camera::LateUpdate(float deltaTime)
{
	assert(!m_Transform.expired());

	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	auto transform = m_Transform.lock();

	Vector3 eye = transform->GetPosition();
	Vector3 to = { 0.0f, 0.0f, 1.0f };
	Vector3 up = transform->GetUp();

	m_MatView = XMMatrixLookToLH(eye, to, up);
}

void Camera::PreRender(Renderer* renderer)
{
	auto transform = m_Transform.lock();
	auto camPos = transform->GetPosition();

	if (!transform)
	{
		LOG_ERROR(L"nullptr : Transform");
		return;
	}

	if (m_bInit)
	{
		VSFixedData vf
		{
			m_MatProjection.Transpose()
		};

		FixedSettings fixedSet
		{
			&vf
		};

		renderer->SetFixed(fixedSet);
		m_bInit = false;
	}

	if (transform->IsChanged())
	{
		VSCameraData vc
		{
			m_MatView.Transpose(),
		};


		auto pos = transform->GetPosition();
		PSCameraData pc
		{
			{pos.x, pos.y, pos.z, 1.0f}
		};

		FrameSettings frameSet
		{
			&vc,
			&pc,
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
