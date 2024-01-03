#pragma once

struct Collision;

class ICollidable
{
public:
	virtual void OnCollisionEnter(Collision* collisionInfo) = 0;
	virtual void OnCollisionStay(Collision* collisionInfo) = 0;
	virtual void OnCollisionExit(Collision* collisionInfo) = 0;
};