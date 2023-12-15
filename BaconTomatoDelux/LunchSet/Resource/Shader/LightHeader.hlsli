//--------------------------------------------------------------------------------------
// Constant Buffer
//--------------------------------------------------------------------------------------
cbuffer CameraData : register(b0) // VS 0
{
    matrix gViewMatrix;
    matrix gProjectionMatrix;
    float4 gWorldCameraPosition;
}

cbuffer ObjectData : register(b1) // VS 1
{
    matrix gWorldMatrix;
}

cbuffer MatrixPallete : register(b2) // VS 2
{
    matrix gMatrixPalleteArray[128];
}

cbuffer LightData : register(b0) // PS 0
{
    float4 gWorldLightDirection;
    float4 gWorldLightColor;
    float gWorldLightIntensity;
}

cbuffer MaterialData : register(b1) // PS 1
{
    uint gShaderScope;
    float gSpecularPower;
    float gOpacityPower;
}

//--------------------------------------------------------------------------------------
// Texture Mapping
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txOpacity : register(t3);
Texture2D txEmissive : register(t4);
Texture2D txMetalness : register(t5);
Texture2D txRoughness : register(t6);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Shader Inputs
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct BVS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD0;
    int4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHTS;
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // 변환된 픽셀 위치
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD0;
    float3 viewDir : TEXCOORD1;
};