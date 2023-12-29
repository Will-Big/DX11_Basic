#pragma once
#include "Component.h"
#include "ILateUpdate.h"
#include "IPreRender.h"

/**
 * \brief	: 단일 카메라 가정
 * \ref		: https://docs.unity3d.com/kr/530/ScriptReference/Camera.html
 * \todo	: 카메라 같은 소수의 컴포넌트를 위해 IPreRender 가 게임오브젝트에 존재하는 것은 불합리함
 *			  IPreRender 와 같은 적게 사용되는 인터페이스를 관리하는 객체를 만들어 원하는 사이클에 실행시키는게 나을듯
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

private:
	bool m_bInit = true;
	Matrix m_MatView = Matrix::Identity;
	Matrix m_MatProjection = Matrix::Identity;

	Vector3 m_Velocity;
};
