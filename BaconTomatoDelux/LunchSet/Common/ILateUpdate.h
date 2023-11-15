#pragma once

class ILateUpdate
{
public:
	virtual ~ILateUpdate() = default;
	virtual void LateUpdate(float deltaTime) = 0;
};