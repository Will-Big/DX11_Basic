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
	~Light() override = default;

	void PreRender(Renderer* renderer) override;
	void GUI() override;

public:
	Vector3 direction = { 1.f, 0.f, 0.f };
	float intensity = 1.0f;
};
