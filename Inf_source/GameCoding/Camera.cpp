#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Component(ComponentType::Camera)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	//S_MatView = GetTransform()->GetWorldMatrix().Invert();

	if (_type == ProjectionType::Perspective)
	{
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.f / 600.f, 1.f, 100.f);
	}
	else
	{
		S_MatProjection = ::XMMatrixOrthographicLH(800.f, 600.f, 0.f, 1.f);
	}
}
