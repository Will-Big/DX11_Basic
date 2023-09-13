#include "Header.hlsli"

PS_INPUT main(float4 Pos : POSITION, float4 Color : COLOR)
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Color = Color;
    return output;
}


/*
셰이더를 미리 로드하고 SetShader의 매개변수만 바꿔주는 것 자체는 성능에 큰 영향을 미치지 않습니다.
그러나 셰이더 교체가 빈번하게 일어나면 그 과정에서 발생하는 상태 변경이 GPU 파이프라인에 부하를 줄 수 있습니다.
이러한 상태 변경은 다음과 같은 이유로 성능에 영향을 미칠 수 있습니다:

파이프라인 플러시:
	셰이더를 교체할 때마다 그래픽스 파이프라인이 플러시될 가능성이 있어,
	이전에 파이프라인에 적재된 작업들이 완료되기를 기다려야 할 수 있습니다.

캐시 무효화:
	셰이더 교체는 GPU의 내부 캐시 무효화를 일으킬 수 있습니다.

드라이버 수준의 작업:
	셰이더 교체는 그래픽스 드라이버 수준에서 추가적인 작업을 필요로 할 수 있습니다.

렌더링 정렬:
	같은 셰이더를 사용하는 렌더링 작업을 묶어서 처리하면 셰이더 교체 횟수를 줄일 수 있습니다.
	이를 통해 상태 변경을 최소화하고 성능을 향상시킬 수 있습니다.

따라서 셰이더를 미리 로드해 놓는 것은 좋지만, 실제 렌더링 과정에서 셰이더를 얼마나 효율적으로 교체하느냐가 성능에 큰 영향을 미칠 수 있습니다.
*/