#pragma once

class ILateUpdate
{
public:
	virtual void LateUpdate(float deltaTime) = 0;
};