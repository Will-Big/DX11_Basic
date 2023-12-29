#pragma once

struct InputStruct;

class IKeyProcessor
{
public:
	virtual ~IKeyProcessor() = default;
	virtual void OnInputProcess(const InputStruct& input) = 0;
};