#include "pch.h"
#include "Animator.h"
#include "GameObject.h"
#include "AnimatorController.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "AnimationClip.h"

Animator::Animator(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

Animator::~Animator()
{
}

void Animator::Update(float deltaTime)
{
	if (controller == nullptr)
		return;

	m_ProgressTime += deltaTime * 15;
	double curFrameTime = m_ProgressTime / (m_CurrentClip->first->duration / m_CurrentClip->first->framePerSecond);

	if(curFrameTime > m_CurrentClip->first->duration)
	{
		if (m_CurrentClip->first->isLoop)
		{
			// 반복
			m_ProgressTime = 0;
			curFrameTime = 0;
		}
		else
		{
			// 상태 변경
			// 시간을 만족하지 않아도 상태 변경(모션 캔슬)이 있을 수 있음
			// todo : 상태 변경 및 상태 변경시 보간
			// info : AnimationController 에 위임 가능?
			
		}
	}

	for(auto& [transfrom, keyFrame] : m_CurrentClip->second)
	{
		transfrom->SetLocalScale(keyFrame->FrameLerp(keyFrame->scaleKeys, curFrameTime));
		transfrom->SetLocalRotation(keyFrame->FrameSlerp(keyFrame->rotationKeys, curFrameTime));
		transfrom->SetLocalPosition(keyFrame->FrameLerp(keyFrame->positionKeys, curFrameTime));
	}
}

void Animator::Initialize()
{
	Component::Initialize();

	std::vector<std::shared_ptr<Transform>> children;
	std::function<void(std::shared_ptr<Transform> obj)> getChildren = [&](std::shared_ptr<Transform> obj)
		{
			for(auto& child : obj->GetChildren())
			{
				children.emplace_back(child.lock());
				getChildren(child.lock());
			}
		};
	getChildren(m_Transform.lock());

	if (controller == nullptr)
	{
		LOG_ERROR(L"nullptr : Animator Controller");
		return;
	}

	KeyFrameMapping(children);

	// todo : 초기 상태로 변경 필요s
	m_CurrentClip = &m_AnimatedObjects[0];
}

void Animator::SetController(std::wstring_view controllerName)
{
	controller = RES_MAN.animatorControllers[controllerName.data()];
}

void Animator::KeyFrameMapping(const std::vector<std::shared_ptr<Transform>>& children)
{
	for(auto& clip : controller->animationClips)
	{
		mappedKeyFrames mapped;

		for(auto& keyFrame : clip.keyFrames)
		{
			for(auto& child : children)
			{
				if(child->GetOwner().lock()->GetName() == keyFrame.name)
				{
					mapped.emplace_back(child, &keyFrame);
					break;
				}
			}
		}

		m_AnimatedObjects.emplace_back(&clip, mapped);
	}
}
