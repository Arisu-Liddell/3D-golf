//定数バッファ
cbuffer MatrixBuffer : register(b0) //スロット番号0
{
    float4x4 mtx; //ワールド行列
}

cbuffer LightBuffer : register(b1) //スロット番号1
{
    bool LightEnable; //ライト有効フラグ
    float3 Dummy0;//この位置ではないとダメ
    //16バイトの境界を超えてしまうため、一つの塊として扱われなくなってしまう
    
    float3 LightDirecition; //光の向き
    float Dummy1; //ダミー　float4の倍数にするため 
}

//float4x4 mtx; //定数バッファで橋渡し



// 頂点シェーダー
float4 main(in float4 position : POSITION0,//入力in　セマンティック
            in float2 texcoord : TEXCOORD0,
            in float3 normal   : NORMAL0,
            out float2 outTexcoord : TEXCOORD0, //出力out
            out float4 outColor : COROR0) : SV_Position
{
    //ライト方向ベクトル
    //float3 lightDirection = float3(0.5, -1.0, 1.0);
    //lightDirection = normalize(lightDirection);//正規化　長さを1にする
    
    //ランバート拡散照明
//   outColor.rgb += float3(0.1, 0.1, 0.4); //環境光　anbient light
     outColor.a = 1.0;
//   outColor.rgb = saturate( - dot(LightDirecition, normal));//dot = 内積
    
    //
    if(LightEnable)
    {
        outColor.rgb = saturate(-dot(LightDirecition, normal)); //dot = 内積
        
        outColor.rgb += float3(0.1, 0.1, 0.4); //環境光　anbient light
    }
    else
    {
        outColor.rgb = 1.0;
    }
    outTexcoord = texcoord;
    return mul(position, mtx);
}



//ランバート拡散照明7
// 
//明るさ = - L * N
//             内積 = (cosΘ)
