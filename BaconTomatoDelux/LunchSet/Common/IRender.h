#pragma once

class Renderer;

class IRender
{
public:
	virtual void Render(Renderer* renderer) = 0;
};