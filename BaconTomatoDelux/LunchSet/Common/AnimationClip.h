#pragma once
#include "Object.h"

/**
 * \brief	: frame 당 transform 정보
 */
struct Vector3Key
{
	double time;
	Vector3 value;

	Vector3Key(double time, const Vector3& value)
		: time(time), value(value) {}
	Vector3Key(double time, float x, float y, float z)
		: time(time), value(x, y, z) {}
};

struct QuaternionKey
{
	double time;
	Quaternion value;

	QuaternionKey(double time, const Quaternion& value)
		: time(time), value(value) {}
	QuaternionKey(double time, float x, float y, float z, float w)
		: time(time), value(x, y, z, w) {}
};


/**
 * \brief	: key frame, assimp 의 aiNodeAnim 과 대응
 */
struct KeyFrames
{
	std::wstring name;

	std::vector<Vector3Key> scaleKeys;
	std::vector<QuaternionKey> rotationKeys;
	std::vector<Vector3Key> positionKeys;

	// info.back().time > time 이 항상 보장됨
	Vector3 FrameLerp(const std::vector<Vector3Key>& info, double time)
	{
		auto frameSet = [&time](auto& info) {return info.time > time; };
		// it = [max] ~ [1]
		auto endFrame = std::find_if(info.begin(), info.end(), frameSet);
		auto beginFrame = endFrame - 1;

		double delta = time - beginFrame->time;
		double total = endFrame->time - beginFrame->time;

		return Vector3::Lerp(beginFrame->value, endFrame->value, static_cast<float>(delta / total));
	}

	Quaternion FrameSlerp(const std::vector<QuaternionKey>& info, double time)
	{
		auto frameSet = [&time](auto& info) {return info.time > time; };
		// it = [max] ~ [1]
		auto it = std::find_if(info.begin(), info.end(), frameSet);

		auto endFrame = std::find_if(info.begin(), info.end(), frameSet);
		auto beginFrame = endFrame - 1;

		double delta = time-  beginFrame->time;
		double total = endFrame->time - beginFrame->time;

		return Quaternion::Slerp(beginFrame->value, endFrame->value, static_cast<float>(delta / total));
	}
};


/**
 * \brief	: 애니메이션 key frame 들을 관리하는 클래스, assimp 의 aiAnimation 과 대응
 * \ref		: https://docs.unity3d.com/ScriptReference/AnimationClip.html
 */
class AnimationClip : public Object
{
public:
	AnimationClip(std::wstring_view name);
	~AnimationClip() override;

public:
	std::vector<KeyFrames> keyFrames;	// cache hit 를 위해서 std::vector 사용
	double duration = 0.0;
	double framePerSecond = 0.0;
	bool isLoop = false;
};
