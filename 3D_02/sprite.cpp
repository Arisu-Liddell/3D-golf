
#include "directx.h"
#include "shader.h"

static ID3D11Buffer* g_VertexBuffer; //頂点バッファ
static const int g_VertexCount = 4; //頂点数


//頂点構造体
struct Vertex
{
	XMFLOAT3 position;//頂点座標　GPUメモリに用意しなくちゃいけない
	XMFLOAT2 texcoord; //テクスチャ座標
};

void SpriteInitialize(void)
{
	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC; //バッファの使用法
	bd.ByteWidth = sizeof(Vertex) * g_VertexCount; //バッファのサイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; //バッファのバインドフラグ
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPUアクセスフラグ

	DirectXGetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer); //バッファの生成

}

void SpriteFinalize(void)
{
	SAFE_RELEASE(g_VertexBuffer); //頂点バッファの解放
}

void SpriteUpdate(void)
{

}

void SpriteDraw(float x, float y, float width, float height, float tx, float ty, float tw, float th ,float rot, float wi,float he)
{
	//頂点バッファにデータを設定
	D3D11_MAPPED_SUBRESOURCE msr;
	DirectXGetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr); //バッファのマッピング

	//マッピングしたバッファにデータを設定
	Vertex* v = (Vertex*)msr.pData; //マッピングしたバッファのポインタを取得
	
	v[0].position = { -width/2,-height/2,  0.0f }; //左上
	v[1].position = { width/2, -height/ 2,  0.0f }; //右上
	v[2].position = { -width/2,height /2   ,0.0f }; //左下
	v[3].position = { width/2, height/2,0.0f }; //右下


	v[0].texcoord = { tx,ty}; //左上
	v[1].texcoord = { tx+tw,ty }; //右上
	v[2].texcoord = { tx,ty+th }; //左下
	v[3].texcoord = { tx + tw,ty + th }; //右下

	DirectXGetDeviceContext()->Unmap(g_VertexBuffer, 0); //バッファのアンマッピング

	//シェーダー設定
	Shader_Begin();

	//シェーダー設定マトリクス
	XMMATRIX matrix = XMMatrixIdentity(); //単位行列を作成


//	if (rot != 0.0f) //回転がある場合
	{
		matrix *= XMMatrixScaling(wi, he, 1.0f); //拡大縮小

		matrix *= XMMatrixRotationZ(rot); //回転

		matrix *= XMMatrixTranslation(x, y, 0.0f); //移動
        //プロジェクしょんマトリクス
		matrix *= XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);; //Z軸を中心に回転
		Shader_SetMatrix(matrix);//単位行列を設定
	}


//	Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(0.0f, 1600.0f, 900.0f, 0.0f, 0.0f, 1.0f));//単位行列を設定

	//頂点バッファを設定
	UINT stride1 = sizeof(Vertex); //頂点のサイズ
	UINT offset = 0; //オフセット
	DirectXGetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride1, &offset); //頂点バッファの設定

	//プリミティブトポロジーの設定
	DirectXGetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //プリミティブトポロジーの設定

	//ポリゴン描画
	DirectXGetDeviceContext()->Draw(g_VertexCount, 0); //ポリゴンの描画 画数
}

