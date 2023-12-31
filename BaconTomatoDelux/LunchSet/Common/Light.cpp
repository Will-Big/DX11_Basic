﻿#include "pch.h"
#include "Light.h"

#include <imgui.h>

#include "Transform.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/PerFrameCB.h"

Light::Light(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

void Light::PreRender(Renderer* renderer)
{
	PSLightData ld
	{
		 {direction.x, direction.y, direction.z, 1.0f},	// todo : Transform
		 {1.0f, 1.0f, 1.0f, 1.0f},	// todo : Color
	};

	FrameSettings frameSet
	{
		nullptr,
		nullptr,
		&ld
	};

	renderer->SetPerFrame(frameSet);

}

void Light::GUI()
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Light Direction", reinterpret_cast<float*>(&direction), 0.01f);
		direction.Normalize();
	}
}
