//定数バッファ
cbuffer MatrixBuffer : register(b0) //スロット番号0
{
    float4x4 Mtx; //ワールド行列
    float4x4 World; //ワールド行列
}

cbuffer LightBuffer : register(b1) //スロット番号1
{
    bool LightEnable; //ライト有効フラグ
    float3 Dummy0;//この位置ではないとダメ
    //16バイトの境界を超えてしまうため、一つの塊として扱われなくなってしまう
    
    float3 LightDirecition; //光の向き
    float Dummy1; //ダミー　float4の倍数にするため 
}
/*
cbuffer ColorBuffer : register(b2)
{
    float4 MulColor; // 乗算カラー
}
*/
//float4x4 mtx; //定数バッファで橋渡し



// 頂点シェーダー
float4 main(in float4 position : POSITION0, //入力in　セマンティック=POSITION0
            in float2 texcoord : TEXCOORD0,
            in float3 normal   : NORMAL0,
            out float2 outTexcoord : TEXCOORD0, //出力out
            out float4 outColor : COROR0) : SV_Position
{
    if(LightEnable)
    {
        //法線ベクトルの座標返還（回転）
        normal = mul(float4(normal, 0.0), World);
        
        //ランバート拡散照明（直接光）
        outColor.rgb = saturate(-dot(LightDirecition, normal))
                        * float3(1.0, 1.0, 0.9); //dot = 内積
        
        //環境光　anbient light
        outColor.rgb += float3(0.3, 0.3, 0.4); 

    }
    else
    {
        outColor.rgb = 1.0;
        outColor.rgb += float3(0.0, 0.0, 0.0);
    }

    outColor.a = 1.0;
    outTexcoord = texcoord;
//    outColor.rgb *= MulColor.rgb;
//    outColor.a *= MulColor.a;


    return mul(position, Mtx);
}



//ランバート拡散照明
// 
//明るさ = - L * N
//             内積 = (cosΘ)
