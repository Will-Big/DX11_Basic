
struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// IA - VS - RS - PS - OM

// Vertex Shader
// ���� ������ ���� �� (��ġ)
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = input.position;
	output.color = input.color;

	return output;
}

// Pixel Shader
// ��� �ȼ� ������ ���� �� (����)
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return input.color;
}