#pragma once

enum class AnimatorConditionMode
{
	If,
	IfNot,
	Greater,
	Less,
	Equals,
	NotEqual,
	None
};

class AnimatorCondition
{
public:
	AnimatorConditionMode mode = AnimatorConditionMode::None;
	std::wstring parameter = {};
	float threshold = 0.0f;
};