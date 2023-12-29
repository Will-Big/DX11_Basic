#pragma once
#include "../Common/GameProcess.h"

// temp Scene
#include "../Common/IKeyProcessor.h"

class Transform;
class Light;

class TesterProcess : public GameProcess, public IKeyProcessor
{
public:
	TesterProcess(const HINSTANCE& hInst);
	~TesterProcess() override;

protected:
	void Update() override;
	void Render(Renderer* renderer) override;
	void ImGuiRender() override;

private:
	void UpdateHW2_Primrose(const InputStruct& input);
	void UpdateHW2_Skinned(const InputStruct& input);
	void ImGuiRenderHW2();

public:
	void OnInputProcess(const InputStruct& input) override;
};
