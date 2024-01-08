#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(std::weak_ptr<GameObject> owner);
	~BoxCollider() override = default;

	void FixedUpdate(float deltaTime) override;
	void GUI() override;
};
