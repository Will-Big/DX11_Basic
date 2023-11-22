#include "LightHeader.hlsli"

PS_INPUT main(BVS_INPUT input)
{
    PS_INPUT output;

    float4 pos = input.position;

    matrix matWorld;

//#ifdef VERTEX_SKINNING
    matWorld = mul(input.blendWeights.x, gMatrixPalleteArray[input.blendIndices.x]);
    matWorld += mul(input.blendWeights.y, gMatrixPalleteArray[input.blendIndices.y]);
    matWorld += mul(input.blendWeights.z, gMatrixPalleteArray[input.blendIndices.z]);
    matWorld += mul(input.blendWeights.w, gMatrixPalleteArray[input.blendIndices.w]);
//#else
    //matWorld = gWorldMatrix; // Static Mesh
//#endif

	// 카메라 벡터
    output.position = mul(input.position, matWorld);
    output.viewDir = normalize(output.position.xyz - gWorldCameraPosition.xyz);

    // 정점 뷰 변환 및 투영 변환
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionMatrix);

    // 노멀 및 탄젠트 벡터 월드 변환 & 노멀라이즈
    output.normal = normalize(mul(input.normal, (float3x3) matWorld));
    output.tangent = normalize(mul(input.tangent, (float3x3) matWorld));

    // 텍스처
    output.uv = input.uv;

    return output;
}