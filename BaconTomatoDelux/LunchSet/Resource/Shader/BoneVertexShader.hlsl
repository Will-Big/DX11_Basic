#include "LightHeader.hlsli"

PS_INPUT main(BVS_INPUT input)
{
    PS_INPUT output;

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
    output.worldPosition = output.position;

    // ���� �� ��ȯ �� ���� ��ȯ
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionMatrix);

    // ź��Ʈ ����
    float3x3 TBN = float3x3(input.tangent, input.biTangent, input.normal);
    output.tangentBasis = mul((float3x3) gWorldMatrix, TBN);

    // �ؽ�ó
    output.uv = input.uv;

    return output;
}