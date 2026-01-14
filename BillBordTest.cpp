#include "directx.h"
#include "BillBordTest.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

static ID3D11Buffer* g_VertexBuffer; //頂点バッファ

static int g_Texture;
static XMFLOAT3 g_Position[2] =
{
	{1.0f, 3.0f,3.0f},
	{-1.0f, 3.0f,3.0f},
};

void BillBordInitialize(void)
{
	//頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DYNAMIC; //バッファの使用法
		bd.ByteWidth = sizeof(Vertex) * 4; //頂点数
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; //バッファのバインドフラグ
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPUアクセスフラグ

		DirectXGetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer); //バッファの生成
	}
	g_Texture = TextureLoad(L"asset\\texture\\Poise.png"); //テクスチャの読み込み
}

void BillBordFinalize(void)
{
	SAFE_RELEASE(g_VertexBuffer); //頂点バッファの解放
}

void BillBordUpdate(void)
{

}

//ポリゴン描画
void BillBordDraw(void)
{
	//テクスチャの設定
	ID3D11ShaderResourceView* texture = GetTexture(g_Texture); //テクスチャの取得
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定

	//頂点バッファを設定
	UINT stride1 = sizeof(Vertex); //頂点のサイズ
	UINT offset = 0; //オフセット
	DirectXGetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride1, &offset);

	//プリミティブトポロジーの設定
	DirectXGetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //トライアングルリスト

	float tx = 0.0f; //テクスチャのX座標
	float ty = 0.0f; //テクスチャのY座標
	float tw = 1.0f; //テクスチャの幅
	float th = 1.0f; //テクスチャの高さ

	//const float w = 1.0f;      // 板の横幅
	//const float h = 3.0f;      // 板の縦幅

	//頂点バッファの設定
	D3D11_MAPPED_SUBRESOURCE msr;//頂点バッファにデータを設定
	DirectXGetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr); //バッファのマッピング

	Vertex* v = (Vertex*)msr.pData; //マッピングしたバッファのポインタを取得

	//const float hw = w * 0.5f;
	//const float hh = h * 0.5f;
		//前面
	v[4].position = { -1.0f	,1.0f	,-1.0f }; //左上
	v[5].position = { 1.0f	,1.0f	,-1.0f }; //右上
	v[6].position = { -1.0f	,-1.0f	,-1.0f }; //左下
	v[7].position = { 1.0f	,-1.0f	,-1.0f }; //右上

	v[0].texcoord = { tx,		ty, }; //左上
	v[1].texcoord = { tx + tw,	ty, }; //右上
	v[2].texcoord = { tx,		ty + th, }; //左下
	v[3].texcoord = { tx + tw,	ty + th, }; //右上

	v[0].normal = { 0.0f,0.0f,-1.0f }; //左上
	v[1].normal = { 0.0f,0.0f,-1.0f }; //右上
	v[2].normal = { 0.0f,0.0f,-1.0f }; //左下
	v[3].normal = { 0.0f,0.0f,-1.0f }; //右上

	DirectXGetDeviceContext()->Unmap(g_VertexBuffer, 0); //バッファのアンマッピング

	for (int i = 0; i < 2; i++)
	{
		//カメラのビュー行列を取得
		XMMATRIX view = GetCameraViewMatrix();
		//ビューの逆行列
		XMMATRIX invView;
		invView = XMMatrixInverse(nullptr, view);//逆行列計算
		//平行移動成分を0にする補正
		invView.r[3].m128_f32[0] = 0.0f;
		invView.r[3].m128_f32[1] = 0.0f;
		invView.r[3].m128_f32[2] = 0.0f;
		//頂点シェーダーに変換行列を設定
		MATRIX matrix;
		matrix.World = XMMatrixIdentity();
		matrix.Mtx = XMMatrixIdentity();
		//拡大マトリクス
		matrix.World *= XMMatrixScaling(0.3f, 1.0f, 1.0f);
		//回転マトリクス
		matrix.World *= invView;
		//移動マトリクス
		matrix.World *= XMMatrixTranslation(g_Position[i].x, g_Position[i].y, g_Position[i].z);
		matrix.Mtx = matrix.World;
		//	ビューマトリクス
		matrix.Mtx *= GetCameraViewMatrix();
		//プロジェクションマトリクス
		matrix.Mtx *= GetCameraProjectionMatrix();
		Shader_SetMatrix(matrix);//シェーダーに行列を設定
		//ポリゴン描画
		DirectXGetDeviceContext()->Draw(4, 0); //ポリゴンの描画 画数
	}
}

