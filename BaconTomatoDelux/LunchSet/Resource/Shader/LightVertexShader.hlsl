#include "LightHeader.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    // ī�޶� ���� (Local -> World)
    output.position = mul(input.position, gWorldMatrix);
    output.worldPosition = output.position;

    // ���� �� ��ȯ �� ���� ��ȯ (World -> View, View -> Projection)
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionMatrix);

    // ź��Ʈ ����
    float3x3 TBN = float3x3(input.tangent, input.biTangent, input.normal);
    output.tangentBasis = mul((float3x3) gWorldMatrix, TBN);

    // �ؽ�ó
    output.uv = input.uv;

    return output;
}