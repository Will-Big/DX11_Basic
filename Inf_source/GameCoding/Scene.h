﻿#pragma once

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();

public:
	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }

private:
	vector<shared_ptr<GameObject>> _gameObjects;
};
