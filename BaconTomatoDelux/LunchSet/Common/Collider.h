#pragma once
#include "Component.h"
#include "IFixedUpdate.h"
#include "Bounds.h"

/**
 * \brief	
 * \ref		: https://docs.unity3d.com/ScriptReference/Collider.html
 */

class Collider : public Component, public IFixedUpdate
{
public:
	Collider(std::weak_ptr<GameObject> owner);
	~Collider() override = default;

protected:
	Bounds bounds; // 하위 객체를 포함하는 Bounding Box 정보
	bool m_bTrigger = false; // Trigger 여부 (사용시 물리적 충돌 X)
};
