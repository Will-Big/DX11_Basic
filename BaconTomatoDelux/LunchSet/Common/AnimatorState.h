#pragma once
#include "Object.h"
#include "AnimatorStateTransition.h"

class AnimatorState : public Object
{
public:
	AnimatorState(std::wstring_view name);
	~AnimatorState() override;

public:
	std::vector<AnimatorStateTransition> transitions;

private:

};
