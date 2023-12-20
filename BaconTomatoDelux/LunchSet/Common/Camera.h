#pragma once
#include "Component.h"
#include "ILateUpdate.h"
#include "IPreRender.h"

/**
 * \brief	: 단일 카메라 가정
 */
class Camera : public Component, public ILateUpdate, public IPreRender
{
public:
	Camera(std::weak_ptr<GameObject> owner);
	~Camera() override;

	void Initialize() override;

	void LateUpdate(float deltaTime) override;
	void PreRender(Renderer* renderer) override;
	void GUI() override;

private:
	void UpdateMatrix();
	
	Matrix m_MatView = Matrix::Identity;
	Matrix m_MatProjection = Matrix::Identity;
};
