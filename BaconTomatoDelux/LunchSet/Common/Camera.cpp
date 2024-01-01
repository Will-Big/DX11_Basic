#include "pch.h"
#include "Camera.h"
#include "Transform.h"

#include "GameProcess.h"

#include "../Graphics/Renderer.h"
#include "../Graphics/FixedCB.h"
#include "../Graphics/PerFrameCB.h"

Camera::Camera(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
	// todo : 카메라 컴포넌트 생성시 관리자에게 등록하기
	//	= 1. 관리자 만들기
	//	= 2. callback 등록 시스템 만들기
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	Component::Initialize();

	m_MatProjection = XMMatrixPerspectiveFovLH(XM_PIDIV2, static_cast<float>(GameProcess::Width) / static_cast<float>(GameProcess::Height), 1.f, 2000.f);
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
	Vector3 to = transform->GetForward();
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

	// info : callback 이 생기면 이것도 한번만 실행할 수 있을 듯
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

	// 카메라 위치 조정 UI
	if (ImGui::DragFloat3("Camera Position", reinterpret_cast<float*>(&camPos), 0.5f))
		m_Transform.lock()->SetPosition(camPos);
}