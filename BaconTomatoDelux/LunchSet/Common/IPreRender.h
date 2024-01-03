#pragma once

class Renderer;

class IPreRender
{
public:
	virtual void PreRender(Renderer* renderer) = 0;
};
