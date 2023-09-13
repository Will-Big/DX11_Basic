#pragma once
#include <directxtk/SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 정점 선언.
struct Vertex
{
	Vector3 position;		// 정점 위치 정보.
	Vector4 color;			// 정점 색상 정보.

	Vertex(float x, float y, float z) : position(x, y, z) { }
	Vertex(Vector3 position) : position(position) { }

	Vertex(Vector3 position, Vector4 color)
		: position(position), color(color) { }
};

struct ConstantBuffer
{
	Matrix world;
	Matrix view;
	Matrix projection;
};

/*
ConstantBuffer를 사용하는 이유는 여러 가지가 있지만, 주로 성능 최적화와 데이터 관리의 편의성 때문입니다.
다음은 몇 가지 주요 이유입니다:

	ConstantBuffer는 일반적으로 월드, 뷰, 투영 행렬을 모두 포함하고 있지만,
	뷰 행렬과 투영 행렬은 대부분의 객체에 대해 동일하게 사용됩니다.
	따라서 이러한 행렬들을 재사용하기 위해 ConstantBuffer에 저장하고,
	월드 행렬만 각 객체마다 업데이트하여 사용하는 것이 일반적입니다.


성능 최적화
	데이터 전송 최소화:
		ConstantBuffer를 사용하면 CPU와 GPU 간에 데이터를 한 번만 전송할 수 있습니다.
		이렇게 하면 매 프레임마다 변환 행렬을 전송하는 것보다 훨씬 효율적입니다.
	배치 처리:
		같은 ConstantBuffer를 사용하는 여러 객체를 그룹화하여 한 번에 렌더링할 수 있습니다.
		이로 인해 드로 콜(Draw Call)의 수가 줄어들고, 성능이 향상됩니다.

데이터 관리의 편의성
	데이터 일관성:
		ConstantBuffer를 사용하면 여러 셰이더에서 동일한 데이터에 액세스할 수 있습니다.
		이렇게 하면 데이터의 일관성을 유지하기 쉽습니다.
	유연성:
		ConstantBuffer는 여러 종류의 데이터(예: 뷰 행렬, 프로젝션 행렬, 라이트 정보 등)를 하나의 버퍼에 저장할 수 있습니다.
		이로 인해 셰이더 코드가 더 간결해지고 유연해집니다.

그래픽스 프로그래밍의 특성
	상태 변경 최소화:
		그래픽스 API에서 상태 변경은 비용이 큰 작업입니다.
		ConstantBuffer를 통해 한 번에 여러 데이터를 업데이트할 수 있으므로, 상태 변경을 최소화할 수 있습니다.
		이러한 이유로, 월드, 뷰, 프로젝션 변환 행렬과 같은 데이터를 ConstantBuffer에 저장하는 것이 일반적입니다.
		이를 통해 CPU와 GPU 간의 통신을 최적화하고, 렌더링 파이프라인을 더 효율적으로 관리할 수 있습니다.
*/