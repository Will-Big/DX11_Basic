#pragma once

#include <directXTK/Mouse.h>
#include <directXTK/Keyboard.h>

constexpr float ROTATION_GAIN = 0.004f;
constexpr float MOVEMENT_GAIN = 0.07f;

class IKeyProcessor;

struct InputStruct
{
	const DirectX::Keyboard::State& keyState;
	const DirectX::Keyboard::KeyboardStateTracker& keyTracker;
	const DirectX::Mouse::State& mouseState;
	const DirectX::Mouse::ButtonStateTracker& mouseTracker;
};

class InputManager
{
public:
	static InputManager* instance;

	bool Initialize(HWND hWnd);
	void Update(float deltaTime);
	void Finalize();

	void AddInputProcessor(IKeyProcessor* inputProcesser);
	void RemoveInputProcesser(IKeyProcessor* inputProcesser);

private:
	std::list<IKeyProcessor*> m_KeyProcessors;

	// input
	std::unique_ptr<Keyboard>       m_Keyboard;
	std::unique_ptr<Mouse>          m_Mouse;
	Keyboard::KeyboardStateTracker	m_KeyboardStateTracker;
	Mouse::ButtonStateTracker		m_MouseStateTracker;

	Mouse::State					m_MouseState;
	Keyboard::State					m_KeyboardState;

private:
	InputManager() = default;
	~InputManager() = default;
};

