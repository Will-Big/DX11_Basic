#pragma once
#include <variant>

/**
 * \brief
 * \ref     : https://docs.unity3d.com/ScriptReference/AnimatorControllerParameter.html
 */

enum class AnimatorControllerParameterType
{
    Float,
    Int,
    Bool,
    Trigger
};

class AnimatorControllerParameter
{
public:
    std::wstring name;
    AnimatorControllerParameterType type;

    std::variant<float, int, bool> value;
};

