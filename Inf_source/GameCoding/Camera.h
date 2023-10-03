#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective,	// 원근 투영
	Orthographic	// 직교 투영
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera();
	~Camera() override;

	void Update() override;
	void SetProjectionType(ProjectionType type) { _type = type; }
	ProjectionType GetProjectionType() { return _type; }

	void UpdateMatrix();

private:
	ProjectionType _type = ProjectionType::Perspective;

public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};
