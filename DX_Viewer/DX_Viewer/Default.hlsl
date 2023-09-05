
struct VS_INPUT
{
	float4 position : POSITION;
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

// IA - VS - RS - PS - OM

// Vertex Shader
// 정점 단위로 실행 됨 (위치)
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = input.position;
	output.uv = input.uv;

	return output;
}


Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

// Pixel Shader
// 모든 픽셀 단위로 실행 됨 (색상)
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 color = texture0.Sample(sampler0, input.uv);

	return color;
}