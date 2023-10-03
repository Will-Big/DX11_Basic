#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
	using Super = Component;

public:
	MonoBehaviour();
	~MonoBehaviour() override;

	void Awake() override;
	void Update() override;
};