#pragma once

#include "Graphics.h"
#include "Pipeline.h"
#include "GameObject.h"

class SceneManager;

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

	shared_ptr<SceneManager> GetSceneManager() { return _scene; }

private:
	HWND _hwnd;

	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;

private:
	shared_ptr<SceneManager> _scene;
};

extern unique_ptr<Game> GGame;