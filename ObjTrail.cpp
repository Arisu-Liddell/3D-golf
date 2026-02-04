#include "directx.h"
#include "ObjTrail.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

static int g_Texture;
static ID3D11Buffer* g_VertexBuffer; //頂点バッファ

#define ObjTrail_LENGTH 30
static XMFLOAT3 g_ObjTrailPosition[ObjTrail_LENGTH];
void ObjTrailInitialize(void)
{
	//頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DYNAMIC; //バッファの使用法
		bd.ByteWidth = sizeof(Vertex) * (ObjTrail_LENGTH * 2 - 2); //頂点数
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; //バッファのバインドフラグ
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPUアクセスフラグ

		DirectXGetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer); //バッファの生成
	}

	g_Texture = TextureLoad(L"asset\\texture\\ObjTrail.png"); //テクスチャの読み込み

}

void ObjTrailFinalize(void)
{
	SAFE_RELEASE(g_VertexBuffer); //頂点バッファの解放
}

void ObjTrailUpdate(void)
{

}

//ポリゴン描画
void ObjTrailDraw(void)
{	//頂点バッファの設定
	{
		D3D11_MAPPED_SUBRESOURCE msr;//頂点バッファにデータを設定
		DirectXGetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr); //バッファのマッピング

		Vertex* v = (Vertex*)msr.pData; //マッピングしたバッファのポインタを取得

		XMFLOAT3 cameraPos = GetCameraPosition();

		for (int i = 0; i < ObjTrail_LENGTH - 1; i++)
		{
			XMFLOAT3 cameraDir;
			cameraDir.x = cameraPos.x - g_ObjTrailPosition[i].x;
			cameraDir.y = cameraPos.y - g_ObjTrailPosition[i].y;
			cameraDir.z = cameraPos.z - g_ObjTrailPosition[i].z;

			XMFLOAT3 ObjTrailDir;
			ObjTrailDir.x = g_ObjTrailPosition[i + 1].x - g_ObjTrailPosition[i].x;
			ObjTrailDir.y = g_ObjTrailPosition[i + 1].y - g_ObjTrailPosition[i].y;
			ObjTrailDir.z = g_ObjTrailPosition[i + 1].z - g_ObjTrailPosition[i].z;

			//外積を求める
			XMFLOAT3 cross;
			cross.x = cameraDir.y * ObjTrailDir.z - cameraDir.z * ObjTrailDir.y;
			cross.y = cameraDir.z * ObjTrailDir.x - cameraDir.x * ObjTrailDir.z;
			cross.z = cameraDir.x * ObjTrailDir.y - cameraDir.y * ObjTrailDir.x;

			//正規化
			float length = sqrtf(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);
			cross.x /= length;
			cross.y /= length;
			cross.z /= length;

			//頂点座標の設定
			v[i * 2 + 0].position = { g_ObjTrailPosition[i].x + cross.x * 0.1f,g_ObjTrailPosition[i].y + cross.y * 0.1f,g_ObjTrailPosition[i].z + cross.z * 0.1f }; //左上
			v[i * 2 + 1].position = { g_ObjTrailPosition[i].x - cross.x * 0.1f,g_ObjTrailPosition[i].y - cross.y * 0.1f,g_ObjTrailPosition[i].z - cross.z * 0.1f }; //右上

			//頂点UVの設定
			float t = (float)i / (float)(ObjTrail_LENGTH - 1);
			v[i * 2 + 0].texcoord = { 0.0f,1.0f - t, }; //左上
			v[i * 2 + 1].texcoord = { 1.0f,1.0f - t, }; //右上

			//頂点法線ベクトルの設定
			v[i * 2 + 0].normal = { 0.0f,0.0f,-1.0f }; //左上
			v[i * 2 + 1].normal = { 0.0f,0.0f,-1.0f }; //右上
		}
		DirectXGetDeviceContext()->Unmap(g_VertexBuffer, 0); //テクスチャの設定
	}
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
	//	ビューマトリクス
	matrix.Mtx *= GetCameraViewMatrix();
	//プロジェクションマトリクス
	matrix.Mtx *= GetCameraProjectionMatrix();

	Shader_SetMatrix(matrix);

	//ポリゴン描画
	DirectXGetDeviceContext()->Draw(ObjTrail_LENGTH * 2 - 2, 0); //ポリゴンの描画 画数
}

void CreateObjTrail(XMFLOAT3 Position)//エフェクト生成
{

}

void SetObjTrailPosition(XMFLOAT3 Position)
{
	//後ろにずらす
	for (int i = 0; i < ObjTrail_LENGTH - 1; i++)
	{
		//一つ前の位置をセット
		g_ObjTrailPosition[i] = g_ObjTrailPosition[i + 1];
	}
	//今のボールの位置をセット
	g_ObjTrailPosition[ObjTrail_LENGTH - 1] = Position;
}

void ResetObjTrailPosition(XMFLOAT3 Position)
{
	//後ろにずらす
	for (int i = 0; i < ObjTrail_LENGTH; i++)
	{
		//一つ前の位置をセット
		g_ObjTrailPosition[i] = Position;
	}
}