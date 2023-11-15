#pragma once
#include "Component.h"
#include "IPreRender.h"

/**
 * \brief	: 단일 라이트 가정
 */

class Light : public Component, public IPreRender
{
public:
	Light(std::weak_ptr<GameObject> owner);
	~Light() override;

	void PreRender(Renderer* renderer) override;
	void GUI() override;

private:

public:
	// temp
	Vector3 direction = { 1.f, 0.f, 0.f };
};
