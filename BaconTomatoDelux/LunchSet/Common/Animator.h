#pragma once
#include "Component.h"
#include "IUpdate.h"

class AnimatorController;
class AnimationClip;
struct KeyFrame;

/**
 * \brief	: Unity Animator 를 지향, 구현상 편의를 위해 일단은 Animation 으로 구현
 */

class Animator : public Component, public IUpdate
{
public:
	Animator(std::weak_ptr<GameObject> owner);
	~Animator() override;

	void Update(float deltaTime) override;

	void Initialize() override;
	void SetController(std::wstring_view controllerName);

private:
	void KeyFrameMapping(const std::vector<std::shared_ptr<Transform>>& children);

public:
	std::shared_ptr<AnimatorController> controller;

private:
	// Component
	using mappedKeyFrames = std::vector<std::pair<std::shared_ptr<Transform>, KeyFrame*>>;
	using ClipAndObjects = std::pair<AnimationClip*, mappedKeyFrames>;
	std::vector<ClipAndObjects> m_AnimatedObjects;

	// Caching
	ClipAndObjects* m_CurrentClip = nullptr;
	float m_ProgressTime = 0.f;
};
