#pragma once
#include "Component.h"
#include "IFixedUpdate.h"

class Collider : public Component, public IFixedUpdate
{
public:
	Collider(std::weak_ptr<GameObject> owner);
	~Collider() override = default;
};
