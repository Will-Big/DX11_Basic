#pragma once

class Renderer;

class IRender
{
public:
	virtual ~IRender() = default;
	virtual void Render(Renderer* renderer) = 0;
};