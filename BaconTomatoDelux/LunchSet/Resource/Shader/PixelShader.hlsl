#include "Header.hlsli"

float4 main(PS_INPUT Input) : SV_TARGET
{
	return float4(Input.Color);
}