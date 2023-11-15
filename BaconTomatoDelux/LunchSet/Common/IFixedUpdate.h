#pragma once

class IFixedUpdate
{
public:
	virtual ~IFixedUpdate() = default;
	virtual void FixedUpdate(float deltaTime) = 0;
};