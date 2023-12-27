#include "LightHeader.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    // 카메라 벡터 (Local -> World)
    output.position = mul(input.position, gWorldMatrix);
    output.worldPosition = output.position;

    // 정점 뷰 변환 및 투영 변환 (World -> View, View -> Projection)
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionMatrix);

    // 탄젠트 공간
    float3x3 TBN = float3x3(input.tangent, input.biTangent, input.normal);
    output.tangentBasis = mul((float3x3) gWorldMatrix, TBN);

    // 텍스처
    output.uv = input.uv;

    return output;
}