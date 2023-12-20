#include "LightHeader.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    // ī�޶� ����
    output.position = mul(input.position, gWorldMatrix);
    output.worldPosition = output.position;

    // ���� �� ��ȯ �� ���� ��ȯ
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionMatrix);

    // ��� �� ź��Ʈ ���� ���� ��ȯ & ��ֶ�����
    output.normal = normalize(mul(input.normal, (float3x3) gWorldMatrix));
    output.tangent = normalize(mul(input.tangent, (float3x3) gWorldMatrix));
    output.biTangent = cross(output.normal, output.tangent);

    // �ؽ�ó
    output.uv = input.uv;

    return output;
}