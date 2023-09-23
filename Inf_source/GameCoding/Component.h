#pragma once

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Update() = 0;

protected:
	shared_ptr<GameObject> GetGameObject() { return _owner.lock(); }
};

