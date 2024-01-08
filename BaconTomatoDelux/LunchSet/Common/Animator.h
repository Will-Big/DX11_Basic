#pragma once
#include "Component.h"
#include "IUpdate.h"

class AnimatorController;
class AnimationClip;
struct KeyFrames;

/**
 * \brief	: Unity Animator 를 지향, 구현상 편의를 위해 일단은 Animation 으로 구현
 * \ref		: https://docs.unity3d.com/ScriptReference/Animator.html
 */

class Animator : public Component, public IUpdate
{
public:
	Animator(std::weak_ptr<GameObject> owner);
	~Animator() override;

	void Initialize() override;
	void Update(float deltaTime) override;
	void GUI() override;

	void SetController(std::wstring_view controllerName);

	// Animator Controller Parameter
	float GetFloat(std::wstring_view name);
	int GetInteger(std::wstring_view name);
	bool GetBool(std::wstring_view name);

	void SetFloat(std::wstring_view name, float value);
	void SetInteger(std::wstring_view name, int value);
	void SetBool(std::wstring_view name, bool value);
	void SetTrigger(std::wstring_view name);
	void ResetTrigger(std::wstring_view name);

private:
	void KeyFrameMapping(const std::vector<std::shared_ptr<Transform>>& children);
	void LoadParameters();

private:
	using mappedKeyFrames = std::vector<std::pair<std::shared_ptr<Transform>, KeyFrames*>>;
	using ClipAndObjects = std::pair<AnimationClip*, mappedKeyFrames>;

	std::vector<ClipAndObjects> m_AnimatedObjects;
	std::shared_ptr<AnimatorController> m_Controller;

	ClipAndObjects* m_CurrentClip = nullptr;
	float m_ProgressTime = 0.f;
};
