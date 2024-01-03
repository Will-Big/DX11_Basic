#pragma once

struct InputStruct;

class IKeyProcessor
{
public:
	virtual void OnInputProcess(const InputStruct& input) = 0;
};