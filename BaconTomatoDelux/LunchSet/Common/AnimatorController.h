#pragma once
#include "Object.h"
#include "AnimationClip.h"

/**
 * \brief	: 상태머신을 이용해 상태에 따라 AnimationClip 을 전환하고 애니메이션을 적절하게 블렌딩 해주는 클래스
 *			  assimp 의 aiAnimation 들을 관리하는 역할
 * \todo	: State Machine 만들기
 * \ref		: https://docs.unity3d.com/ScriptReference/Animations.AnimatorController.html
 */

class AnimatorController : public Object
{
public:
	AnimatorController(std::wstring_view name);
	~AnimatorController() override;


public:
	std::vector<AnimationClip> animationClips;
};