#pragma once
#include <variant>

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

