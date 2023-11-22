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

	// ī�޶� ����
    output.position = mul(input.position, matWorld);
    output.viewDir = normalize(output.position.xyz - gWorldCameraPosition.xyz);

    // ���� �� ��ȯ �� ���� ��ȯ
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionMatrix);

    // ��� �� ź��Ʈ ���� ���� ��ȯ & ��ֶ�����
    output.normal = normalize(mul(input.normal, (float3x3) matWorld));
    output.tangent = normalize(mul(input.tangent, (float3x3) matWorld));

    // �ؽ�ó
    output.uv = input.uv;

    return output;
}