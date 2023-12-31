#include "LightHeader.hlsli"

#define DIFFUSE 1                   // 0000001 (= ALBEDO)
#define NORMAL (DIFFUSE << 1)       // 0000010
#define SPECULAR (NORMAL << 1)      // 0000100
#define OPACITY (SPECULAR << 1)     // 0001000
#define EMISSIVE (OPACITY << 1)     // 0010000
#define METALNESS (EMISSIVE << 1)   // 0100000
#define ROUGHNESS (METALNESS << 1)  // 1000000

#define DEFAULT_COLOR (float4(1, 1, 1, 1))

static const float PI = 3.141592f;
static const float Epsilon = 0.0001f;
static const float3 Fdielectric = { 0.04f, 0.04f, 0.04f };

float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float opacity = 1.0f;
    if (gTextureBitmask & OPACITY)
    {
        opacity = txOpacity.Sample(samLinear, input.uv).a;
        if (opacity <= 0.0f)
            discard;
    }

    float3 albedo = DEFAULT_COLOR;
    if (gTextureBitmask & DIFFUSE)
    {
        albedo = pow(txDiffuse.Sample(samLinear, input.uv).rgb, 2.2f);
    }

    float3 ambient = albedo * 0.3f;

    float3 normal = 0.f;
    if (gTextureBitmask & NORMAL)
    {
        normal = normalize(2.0f * txNormal.Sample(samLinear, input.uv).rgb - 1.0f);
        normal = normalize(mul(normal, input.tangentBasis));
    }

    float3 viewDir = normalize(gWorldCameraPosition - input.worldPosition);
    float3 halfVec = normalize(viewDir - gWorldLightDirection);
    float cosNH = max(0.f, dot(normal, halfVec));
    float cosNL = max(0.f, dot(normal, -gWorldLightDirection));

    float3 specular = 0.f;

    if (gTextureBitmask & SPECULAR)
    {
		specular = pow(cosNH, gSpecularPower); // todo : delete
        float4 specularIntensity = txSpecular.Sample(samLinear, input.uv);
        specular = specular * specularIntensity;
    }
    
    float3 directLighting = 0.0f;
    if ((gTextureBitmask & METALNESS) && (gTextureBitmask & ROUGHNESS))
    {
        float cosLH = max(0.f, dot(halfVec, viewDir));
        float cosNV = max(0.f, dot(normal, viewDir));

        float metalness = txMetalness.Sample(samLinear, input.uv);
        float3 F0 = lerp(Fdielectric, albedo, metalness);
        float3 F = fresnelSchlick(F0, cosLH);
        float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

        float roughness = txRoughness.Sample(samLinear, input.uv);
        float D = ndfGGX(cosNH, max(0.1f, roughness));
        float G = gaSchlickGGX(cosNL, cosNV, roughness);

        // BRDF
        float3 diffuse = kd * albedo;
        specular = (F * D * G) / max(Epsilon, 4.0 * cosNL * cosNV);
        directLighting += (diffuse + specular) * gWorldLightColor * cosNL;
    }

    float4 finalColor = float4((ambient + directLighting).rgb, opacity);
    finalColor.rgb = pow(finalColor.rgb, 1 / 2.2f);

    return finalColor;
}