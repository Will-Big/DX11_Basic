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

	Vector3 FrameLerp(const std::vector<Vector3Key>& keys, double time) {
		return FrameInterpolate<Vector3, Vector3Key>(keys, time, Vector3::Lerp);
	}

	Quaternion FrameSlerp(const std::vector<QuaternionKey>& keys, double time) {
		return FrameInterpolate<Quaternion, QuaternionKey>(keys, time, Quaternion::Slerp);
	}

private:
	template<typename T, typename KeyType>
	T FrameInterpolate(const std::vector<KeyType>& keys, double time, T(*interpolateFunc)(const T&, const T&, float))
	{
		assert(!keys.empty() && keys.back().time > time);

		auto frameSet = [&time](const auto& key) { return key.time > time; };
		auto endFrame = std::find_if(keys.begin(), keys.end(), frameSet);
		auto beginFrame = endFrame - 1;

		double delta = time - beginFrame->time;
		double total = endFrame->time - beginFrame->time;

		return interpolateFunc(beginFrame->value, endFrame->value, static_cast<float>(delta / total));
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
