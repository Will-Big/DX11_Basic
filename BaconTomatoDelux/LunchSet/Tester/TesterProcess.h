#pragma once
#include "../Common/GameProcess.h"

class Transform;
class Light;

class TesterProcess : public GameProcess
{
public:
	TesterProcess(const HINSTANCE& hInst);
	~TesterProcess() override;

protected:
	void Update() override;
	void Render(Renderer* renderer) override;
	void ImGuiRender() override;


private:
	void UpdateHW2();
	void ImGuiRenderHW2();
};
