#pragma once
#include "AnimatorStateMachine.h"

/**
 * \brief	: ���� �ӽ��� �����ϴ� ��ü, ���� ����ŷ�Ͽ� ��ü�� �Ϻθ� �ִϸ��̼��� ������ �� �ְ� ��
 * \todo	: ����ŷ
 * \ref		: https://docs.unity3d.com/ScriptReference/Animations.AnimatorControllerLayer.html
 */

class AnimatorControllerLayer
{
public:
	std::wstring name;

	AnimatorStateMachine stateMachine;
};

