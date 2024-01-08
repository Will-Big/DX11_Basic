#include "pch.h"
#include "Collider.h"

Collider::Collider(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}
