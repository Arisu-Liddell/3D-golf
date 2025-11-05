//定数バッファ
float4x4 mtx;

// 頂点シェーダー
float4 main(in float4 position : POSITION0,//セマンティック
            in float2 texcoord : TEXCOORD0,
            in float3 normal   : NORMAL0,
            out float2 outTexcoord : TEXCOORD0) : SV_Position
{
    outTexcoord = texcoord;
    return mul(position, mtx);
}