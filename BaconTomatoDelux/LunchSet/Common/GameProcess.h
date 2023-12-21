#pragma once

class Renderer;
class Graphics;
class GameObject;

class GameProcess
{
public:
	GameProcess(HINSTANCE hInst, std::wstring_view title, int width, int height, bool bImGuiInit);
	virtual ~GameProcess();

	void Loop();

protected:
	virtual void Update();
	virtual void Render(Renderer* renderer);
	virtual void ImGuiRender();

protected:
	const std::wstring m_Title;
	HWND m_hWnd = nullptr;
	bool m_bImGuiRender = true;

	std::unique_ptr<Graphics> m_Graphics;
	std::unique_ptr<Renderer> m_Renderer;

public:
	static uint32_t Width;
	static uint32_t Height;

	// temp (Scene 으로 이동)
	static std::vector<std::shared_ptr<GameObject>> m_GameObjects;
};

/*
 *	https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
 *	https://github.com/Wuszt/Forge
 *	https://www.3dgep.com/forward-plus/
 *
 *	https://www.gamedev.net/tutorials/programming/graphics/opengl-batch-rendering-r3900/
*/