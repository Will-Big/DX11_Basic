#pragma once

class Renderer;

class IPreRender
{
public:
	virtual ~IPreRender() = default;
	virtual void PreRender(Renderer* renderer) = 0;
};
