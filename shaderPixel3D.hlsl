Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

float4 main(in float4 position : SV_Position,
            in float2 texcoord : TEXCOORD0,
            in float4 color : COLOR0) : SV_TARGET
{
    float4 outColor = tex.Sample(samplerState, texcoord) * color;
    
    //clip(outColor.a - 0.1); //アルファクリップテスト
    
    return outColor;
}