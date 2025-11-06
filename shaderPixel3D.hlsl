Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

float4 main(in float4 position : SV_Position,
            in float2 texcoord : TEXCOORD0,
            in float4 color : COLOR0) : SV_TARGET
{
    return tex.Sample(samplerState, texcoord) * color;
}

// return float4(255.0f, 75.0f, 0.0f, 1.0f); 