#pragma once

#include <directXTK/Mouse.h>
#include <directXTK/Keyboard.h>

constexpr float ROTATION_GAIN = 0.004f;
constexpr float MOVEMENT_GAIN = 0.07f;

struct InputStruct
{
	const Keyboard::State& keyState;
	const Keyboard::KeyboardStateTracker& keyTracker;
	const Mouse::State& mouseState;
	const Mouse::ButtonStateTracker& mouseTracker;
};

class IKeyProcessor
{
public:
	virtual void OnInputProcess(const InputStruct& input) = 0;
};

class InputManager
{
public:
	static InputManager* instance;

	std::list<IKeyProcessor*> m_InputProcessers;

	// input
	std::unique_ptr<Keyboard>       m_Keyboard;
	std::unique_ptr<Mouse>          m_Mouse;
	Keyboard::KeyboardStateTracker	m_KeyboardStateTracker;
	Mouse::ButtonStateTracker		m_MouseStateTracker;

	Mouse::State					m_MouseState;
	Keyboard::State					m_KeyboardState;

	bool Initialize(HWND hWnd);
	void Update(float deltaTime);
	void Finalize();

	void AddInputProcesser(IKeyProcessor* inputProcesser);
	void RemoveInputProcesser(IKeyProcessor* inputProcesser);

private:
	InputManager() = default;
	~InputManager() = default;
};

