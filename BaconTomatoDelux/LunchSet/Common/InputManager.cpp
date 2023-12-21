#include "pch.h"
#include "InputManager.h"

#include "GameProcess.h"

InputManager::InputManager()
{
	width = static_cast<float>(GameProcess::Width);
	height = static_cast<float>(GameProcess::Height);
}

void InputManager::Initialize()
{
	for (auto& key : m_keyState)
	{
		key.keyState = KeyState::NONE;
		key.prevPushed = false;
	}
}

void InputManager::Update()
{
	HWND hWnd = GetFocus();

	if (nullptr != hWnd)
	{
		for (int i = 0; i < 256; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (m_keyState[i].prevPushed)
				{
					m_keyState[i].keyState = KeyState::HOLD;
				}
				else
				{
					m_keyState[i].keyState = KeyState::DOWN;
					m_keyState[i].prevPushed = true;
				}
			}
			else
			{
				if (m_keyState[i].prevPushed)
				{
					m_keyState[i].keyState = KeyState::UP;
					m_keyState[i].prevPushed = false;
				}
				else
				{
					m_keyState[i].keyState = KeyState::NONE;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 256; i++)
		{
			m_keyState[i].prevPushed = false;
			if (KeyState::DOWN == m_keyState[i].keyState || KeyState::HOLD == m_keyState[i].keyState)
			{
				m_keyState[i].keyState = KeyState::UP;
			}
			else if (KeyState::UP == m_keyState[i].keyState)
			{
				m_keyState[i].keyState = KeyState::NONE;
			}
		}
	}

	// Mouse 위치 계산
	POINT ptPos = {};
	// 현재 마우스 위치
	GetCursorPos(&ptPos);
	// 스크린좌표계로 마우스 위치 반환
	ScreenToClient(hWnd, &ptPos);
	m_curMousePos = Vector2((float)ptPos.x, (float)ptPos.y);
}


