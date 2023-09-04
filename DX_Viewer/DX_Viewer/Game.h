#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hWnd);
	void Update();
	void Render();

private:
	HWND _hWnd;
	uint32 _width = 0;
	uint32 _height = 0;

private:
};
