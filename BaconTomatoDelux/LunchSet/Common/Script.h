#pragma once
#include "Component.h"
#include "IUpdate.h"
#include "ILateUpdate.h"
#include "IFixedUpdate.h"

class Script : public Component, public IUpdate, public IFixedUpdate, public ILateUpdate
{
public:
	Script(std::weak_ptr<GameObject> owner) : Component(owner) {}
	~Script() override = default;

	void Update(float deltaTime) override {}
	void LateUpdate(float deltaTime) override {}
	void FixedUpdate(float deltaTime) override {}
};
