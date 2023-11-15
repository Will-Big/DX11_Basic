#include "Header.hlsli"

PS_INPUT main( VS_INPUT Input )
{
    PS_INPUT Output;

    Output.Position = Input.Position;
    Output.Color = Input.Color;

    return Output;
}