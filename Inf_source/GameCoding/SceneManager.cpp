#include "pch.h"
#include "SceneManager.h"

#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Transform.h"

SceneManager::SceneManager(shared_ptr<Graphics> graphics)
	: _graphics(graphics)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	if (_activateScene == nullptr)
		return;

	_activateScene->Awake();
	_activateScene->Start();
}

void SceneManager::Update()
{
	if (_activateScene == nullptr)
		return;

	_activateScene->Update();
	_activateScene->LateUpdate();

	_activateScene->FixedUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	_activateScene = LoadTestScene();
	Init();
}


shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

	shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		camera->GetOrAddTransform();
		camera->AddComponent(make_shared<Camera>());
		scene->AddGameObject(camera);
	}

	shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		monster->GetOrAddTransform();
		monster->AddComponent(make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext()));
		// ..
		scene->AddGameObject(monster);
	}

	return scene;
}
