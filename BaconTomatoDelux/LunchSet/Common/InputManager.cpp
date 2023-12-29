#include "pch.h"
#include "InputManager.h"
#include "IKeyProcessor.h"

InputManager* InputManager::instance = new InputManager{};

bool InputManager::Initialize(HWND hWnd)
{
	m_Keyboard = std::make_unique<Keyboard>();
	m_Mouse = std::make_unique<Mouse>();
	m_Mouse->SetWindow(hWnd);
	return true;
}

void InputManager::Update(float deltaTime)
{
	m_MouseState = m_Mouse->GetState();
	m_MouseStateTracker.Update(m_MouseState);

	m_KeyboardState = m_Keyboard->GetState();
	m_KeyboardStateTracker.Update(m_KeyboardState);

	for (auto& it : m_KeyProcessors)
	{
		(it)->OnInputProcess({m_KeyboardState, m_KeyboardStateTracker, m_MouseState, m_MouseStateTracker});
	}
}

void InputManager::Finalize()
{
	delete instance;
}

void InputManager::AddInputProcessor(IKeyProcessor* inputProcesser)
{
	m_KeyProcessors.push_back(inputProcesser);
}

void InputManager::RemoveInputProcesser(IKeyProcessor* inputProcesser)
{
	m_KeyProcessors.remove(inputProcesser);
}
