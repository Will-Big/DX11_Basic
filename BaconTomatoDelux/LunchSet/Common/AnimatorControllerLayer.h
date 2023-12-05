#pragma once
#include "AnimatorStateMachine.h"

/**
 * \brief	: 상태 머신을 소유하는 객체, 모델을 마스킹하여 신체의 일부만 애니메이션을 조절할 수 있게 함
 * \todo	: 마스킹
 * \ref		: https://docs.unity3d.com/ScriptReference/Animations.AnimatorControllerLayer.html
 */

class AnimatorControllerLayer
{
public:
	std::wstring name;

	AnimatorStateMachine stateMachine;
};

