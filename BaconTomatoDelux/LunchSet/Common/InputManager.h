#pragma once

#define INPUT_MAN InputManager::GetInstance()

class InputManager
{
private:
	enum class KeyState
	{
		NONE,	  // 현재 및 이전	프레임에 입력 없음
		DOWN,	  // 현재			프레임에 입력 됨
		HOLD,	  // 현재 및 이전	프레임에 입력 됨
		UP,		  // 현재			프레임에 입력 종료함
	};

	struct KeyInfo
	{
		bool prevPushed;
		KeyState keyState;
	};

private:
	InputManager();
	~InputManager() = default;

private:
	float width = 0.f;
	float height = 0.f;

	KeyInfo m_keyState[256] {};	// Windows 키 코드는 0 ~ 255
	Vector2 m_curMousePos;

public:
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	void Initialize();
	void Update();

public: // 키 입력 진행 순서는 아래 함수를 따름
	inline bool IsKeyNone(int keyCode) const;
	inline bool IsKeyDown(int keyCode) const;
	inline bool IsKeyHold(int keyCode) const;
	inline bool IsKeyUp(int keyCode) const;
	inline Vector2 GetMousePos() const;
};

inline bool InputManager::IsKeyNone(int keyCode) const
{
	return m_keyState[keyCode].keyState == KeyState::NONE;
}

inline bool InputManager::IsKeyDown(int keyCode) const
{
	return m_keyState[keyCode].keyState == KeyState::DOWN;
}

inline bool InputManager::IsKeyHold(int keyCode) const
{
	return m_keyState[keyCode].keyState == KeyState::HOLD;
}

inline bool InputManager::IsKeyUp(int keyCode) const
{
	return m_keyState[keyCode].keyState == KeyState::UP;
}

inline Vector2 InputManager::GetMousePos() const
{
	return { m_curMousePos.x - width / 2, -(m_curMousePos.y - height / 2) };
}

