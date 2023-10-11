#pragma once

class Scene;

class SceneManager
{
public:
	SceneManager(shared_ptr<Graphics> graphics);
	~SceneManager();

	void Init();
	void Update();
	void LoadScene(wstring sceneName);

public:
	shared_ptr<Scene> GetActivateScene() { return _activateScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Graphics> _graphics;

private:
	shared_ptr<Scene> _activateScene;
};

