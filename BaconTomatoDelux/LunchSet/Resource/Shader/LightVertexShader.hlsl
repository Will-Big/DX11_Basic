#include "LightHeader.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    // 카메라 벡터
    output.position = mul(input.position, gWorldMatrix);
    output.worldPosition = output.position;

    // 정점 뷰 변환 및 투영 변환
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionMatrix);

    // 노멀 및 탄젠트 벡터 월드 변환 & 노멀라이즈
    output.normal = normalize(mul(input.normal, (float3x3) gWorldMatrix));
    output.tangent = normalize(mul(input.tangent, (float3x3) gWorldMatrix));
    output.biTangent = cross(output.normal, output.tangent);

    // 텍스처
    output.uv = input.uv;

    return output;
}