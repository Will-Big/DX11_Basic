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


/**
 *	Skinning Animation : 정점(Vertex)이 관절(Bone)에 붙어서 움직이는 Animation
 *		Bone
 *		Vertex
 *			어떤 Bone 을 따라갈지 정보를 줘야 함 (뼈에 붙은 고기처럼)
 *			최대 4개의 Bone 에 영향을 받는데 각각의 가중치를 다르게 줌
 */