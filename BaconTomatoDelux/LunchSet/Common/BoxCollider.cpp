#include "pch.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(std::weak_ptr<GameObject> owner)
	: Collider(owner)
{
}

void BoxCollider::FixedUpdate(float deltaTime)
{
}

void BoxCollider::GUI()
{
	
}
