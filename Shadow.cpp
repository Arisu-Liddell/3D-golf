#include "directx.h"
#include "Shadow.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "field.h"
static int g_Texture;
static ID3D11Buffer* g_VertexBuffer; //頂点バッファ
static XMFLOAT3 g_Position;

void ShadowHitCheck(void);

void ShadowInitialize(void)
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
	//頂点バッファの設定
	{
		D3D11_MAPPED_SUBRESOURCE msr;//頂点バッファにデータを設定
		DirectXGetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr); //バッファのマッピング

		Vertex* v = (Vertex*)msr.pData; //マッピングしたバッファのポインタを取得

		//上面
		v[0].position = { -0.5f	,0.0f	,0.5f }; //左上
		v[1].position = { 0.5f	,0.0f	,0.5f }; //右上
		v[2].position = { -0.5f	,0.0f	,-0.5f }; //左下
		v[3].position = { 0.5f	,0.0f	,-0.5f }; //右上

		//上面
		v[0].normal = { 0.0f,1.0f,0.0f }; //左上
		v[1].normal = { 0.0f,1.0f,0.0f }; //右上
		v[2].normal = { 0.0f,1.0f,0.0f }; //左下
		v[3].normal = { 0.0f,1.0f,0.0f }; //右上

		float tx = 0.0f;
		float ty = 0.0f;
		float tw = 1.0f;
		float th = 1.0f;

		//上面
		v[0].texcoord = { tx,		ty, }; //左上
		v[1].texcoord = { tx + tw,	ty, }; //右上
		v[2].texcoord = { tx,		ty + th, }; //左下
		v[3].texcoord = { tx + tw,	ty + th, }; //右上

		DirectXGetDeviceContext()->Unmap(g_VertexBuffer, 0); //バッファのアンマッピング
	}
	//テクスチャの読み込み
	g_Texture = TextureLoad(L"asset\\texture\\shadow.png"); //テクスチャの読み込み

	g_Position = XMFLOAT3(-1.0f, 0.0f, 0.0f);
}

void ShadowFinalize(void)
{
	SAFE_RELEASE(g_VertexBuffer); //頂点バッファの解放
}

void ShadowUpdate(void)
{

}

//ポリゴン描画
void ShadowDraw(void)
{
	if (g_Position.y > -30.0)
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

		//頂点シェーダーに変換行列を設定
		MATRIX matrix;
		matrix.World = XMMatrixIdentity();
		matrix.Mtx = XMMatrixIdentity();

		//移動マトリクス
		matrix.World *= XMMatrixTranslation(g_Position.x, g_Position.y, g_Position.z);

		matrix.Mtx = matrix.World;
		//	ビューマトリクス
		matrix.Mtx *= GetCameraViewMatrix();
		//プロジェクションマトリクス
		matrix.Mtx *= GetCameraProjectionMatrix();

		Shader_SetMatrix(matrix);

		//ポリゴン描画
		DirectXGetDeviceContext()->Draw(4, 0); //ポリゴンの描画 画数
	}
}

void SetShadowPosition(XMFLOAT3 Position)
{
	//今のボールの位置をセット
	g_Position = Position;
	ShadowHitCheck();

}

void ShadowHitCheck(void)
{
	BLOCK* block = GetFieldBlock();

	float BlockRadius = 0.5f;
	float ShadowY = -100.0f;

	for (int i = 0; i < GridCount; i++)
	{
		if (block[i].Position.z - BlockRadius < g_Position.z && g_Position.z < block[i].Position.y + BlockRadius) + 0.001f;
		{
			if (block[i].Position.x - BlockRadius < g_Position.x && g_Position.x < block[i].Position.x + BlockRadius)
			{
				if (block[i].Position.y + BlockRadius < g_Position.y)//ブロックの上にいるか？
				{
					float y = (block[i].Position.y + BlockRadius) +0.001f;
					
					if (y > ShadowY)
						ShadowY = y;
				}
			}
		}
	}
	g_Position.y = ShadowY;
}