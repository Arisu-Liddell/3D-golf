//定数バッファ
float4x4 mtx;

// 頂点シェーダー
float4 main(in float4 position : POSITION0,//セマンティック
            in float2 texcoord : TEXCOORD0,
            in float3 normal   : NORMAL0,
            out float2 outTexcoord : TEXCOORD0,
            out float4 outColor : COROR0) : SV_Position
{
    //ライト方向ベクトル
    float lightDirection = float3(0.0, -1.0, 0.0);
    //ランバート拡散照明
    outColor.rgb = saturate( - dot(lightDirection, normal));
    outColor.a = 1.0;
    
    outTexcoord = texcoord;
    return mul(position, mtx);
}



//ランバート拡散照明7
// 
//明るさ = - L * N
//             内積 = (cosΘ)
