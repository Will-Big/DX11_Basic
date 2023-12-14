#include "LightHeader.hlsli"

#define DIFFUSE 1                   // 0000001
#define NORMAL (DIFFUSE << 1)       // 0000010
#define SPECULAR (NORMAL << 1)      // 0000100
#define OPACITY (SPECULAR << 1)     // 0001000
#define EMISSIVE (OPACITY << 1)     // 0010000
#define METALNESS (EMISSIVE << 1)   // 0100000
#define ROUGHNESS (METALNESS << 1)  // 1000000

#define DEFAULT_COLOR (float4(1, 1, 1, 1))

float4 main(PS_INPUT input) : SV_TARGET
{
    float opacity = 1.0f;
    if (gShaderScope & OPACITY)
    {
        opacity = txOpacity.Sample(samLinear, input.uv).a;
        if (opacity <= 0.0f)
            discard;
    }

    float4 baseColor = DEFAULT_COLOR;
    if (gShaderScope & DIFFUSE)
    {
        baseColor = pow(txDiffuse.Sample(samLinear, input.uv), 2.2f);
    }

    float4 ambient = baseColor * 0.3f;

    float3 normal = normalize(input.normal);
    if (gShaderScope & NORMAL)
    {
        float3 tangent = normalize(input.tangent);
        float3 biTangent = cross(normal, tangent);
        float3 normalTangentSpace = txNormal.Sample(samLinear, input.uv).rgb * 2.0f - 1.0f;
        float3x3 worldTransform = float3x3(tangent, biTangent, normal);
        normal = normalize(mul(normalTangentSpace, worldTransform));
    }

    float NDotL = max(0, dot(normal, gWorldLightDirection.xyz));
    float4 diffuse = NDotL * baseColor;

    float3 viewDir = normalize(input.viewDir);
    float3 halfVec = normalize(viewDir + gWorldLightDirection.xyz);
    float HDotN = max(0, dot(halfVec, normal));

    float4 specular = 0.f;
    specular = pow(HDotN, gSpecularPower);
    if ((gShaderScope & SPECULAR) && diffuse.x > 0.f)
    {
        float4 specularIntensity = txSpecular.Sample(samLinear, input.uv);
        specular = specular * specularIntensity;
    }

    float4 finalColor = float4((ambient + diffuse + specular).rgb, opacity);
    finalColor.rgb = pow(finalColor.rgb, 1 / 2.2f);

    return finalColor;
}