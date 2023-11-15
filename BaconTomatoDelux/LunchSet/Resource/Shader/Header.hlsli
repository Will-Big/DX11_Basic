// Syntax : https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-language-syntax


struct VS_INPUT
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};