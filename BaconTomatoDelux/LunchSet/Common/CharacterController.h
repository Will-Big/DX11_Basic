#pragma once
#include "Collider.h"

class CharacterController : public Collider
{
public:
	CharacterController(std::weak_ptr<GameObject> owner);
	~CharacterController() override = default;


private:

};
