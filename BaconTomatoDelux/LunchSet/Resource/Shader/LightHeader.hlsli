//--------------------------------------------------------------------------------------
// Constant Buffer
//--------------------------------------------------------------------------------------
cbuffer CameraData : register(b0) // VS 0
{
    matrix gViewMatrix;             // once updated per frame
    matrix gProjectionMatrix;       // rarely updated
    float4 gWorldCameraPosition;    // once updated per frame (need in PS)
}

cbuffer ObjectData : register(b1) // VS 1
{
    matrix gWorldMatrix;             // always updated per object
}

cbuffer MatrixPallete : register(b2) // VS 2
{
    matrix gMatrixPalleteArray[128]; // always updated per skinned object
}

cbuffer LightData : register(b0) // PS 0
{
    float4 gWorldLightDirection;     // once updated per frame
    float4 gWorldLightColor;         // once updated per frame
    float gWorldLightIntensity;      // once updated per frame
}

cbuffer MaterialData : register(b1) // PS 1
{
    uint gShaderScope;               // always updated per object
    float gSpecularPower;            // always updated per object
    float gOpacityPower;             // always updated per object
}

//--------------------------------------------------------------------------------------
// Texture Mapping
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txOpacity : register(t3);
Texture2D txEmissive : register(t4);

// PBR
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
    float3 biTangent : BITANGENT;
    float2 uv : TEXCOORD0;
    float3 viewDir : TEXCOORD1;
};