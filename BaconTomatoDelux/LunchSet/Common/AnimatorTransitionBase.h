#pragma once
#include "Object.h"
#include "AnimatorContidion.h"

class AnimatorState;
class AnimatorStateMachine;

/**
 * \brief	: 트랜지션 부모 클래스. 모든 조건이 충족되면 트랜지션이 발생.
 * \ref		: https://docs.unity3d.com/ScriptReference/Animations.AnimatorTransitionBase.html
 */

class AnimatorTransitionBase : public Object
{
public:
	AnimatorTransitionBase(std::wstring_view name);
	~AnimatorTransitionBase() override;

	std::vector<AnimatorCondition> conditions;
	AnimatorState* destinationState;
	AnimatorStateMachine* destinationStateMachine;
};
