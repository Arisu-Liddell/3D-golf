#include "cube.h"
#include "directx.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"



static ID3D11Buffer* g_VertexBuffer; //頂点バッファ
static ID3D11Buffer* g_IndexBuffer; //インデックスバッファ


//頂点構造体
struct Vertex
{
	XMFLOAT3 position;//頂点座標　GPUメモリに用意しなくちゃいけない
	XMFLOAT2 texcoord; //テクスチャ座標
};

static const int g_VertexCount = 8; //頂点数
static int g_Texture;
static XMFLOAT3 g_Rotation;

void CubeInitialize(void)
{
	//頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DYNAMIC; //バッファの使用法
		bd.ByteWidth = sizeof(Vertex) * 8; //頂点数
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; //バッファのバインドフラグ
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPUアクセスフラグ

		DirectXGetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer); //バッファの生成
	}

	//頂点バッファの設定
	{
		D3D11_MAPPED_SUBRESOURCE msr;//頂点バッファにデータを設定
		DirectXGetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr); //バッファのマッピング

		Vertex* v = (Vertex*)msr.pData; //マッピングしたバッファのポインタを取得

		float width = 300.0f;
		float height = 300.0f;

		v[0].position = { -5.0f	,5.0f	,5.0f }; //左上
		v[1].position = { 5.0f	,5.0f	,5.0f }; //右上
		v[2].position = { -5.0f	,5.0f	,-5.0f }; //左下
		v[3].position = { 5.0f	,5.0f	,-5.0f }; //右上

		v[4].position = { -5.0f	,-5.0f	,5.0f }; //左上
		v[5].position = { 5.0f	,-5.0f	,5.0f }; //右上
		v[6].position = { -5.0f	,-5.0f	,-5.0f }; //左下
		v[7].position = { 5.0f	,-5.0f	,-5.0f }; //右上
		//2345

		////手前
		//v[6].position = { -5.0f,0.0f,	-5.0f };//左下
		//v[7].position = { 5.0f,	0.0f,	-5.0f };//右下
		//v[8].position = { -5.0f,-5.0f,	-5.0f }; //左下

		//v[9].position = { 5.0f,	0.0f,	-5.0f }; //右上
		//v[10].position ={ 5.0f,-5.0f,	-5.0f }; //右下
		//v[11].position ={ -5.0f,	-5.0f,-5.0f };//左下

		////下面
		//v[12].position = { 5.0f,	-5.0f,	-5.0f };//右下
		//v[13].position = { 5.0f,	-5.0f,	5.0f };//右上
		//v[14].position = { -5.0f,	-5.0f,	-5.0f };//左下

		//v[15].position = { 5.0f,	-5.0f,	5.0f };//右上
		//v[16].position = { -5.0f,	-5.0f,	5.0f };//左上
		//v[17].position = { -5.0f,	-5.0f,	-5.0f };//左下

		////逆左面
		//v[18].position = { -5.0f,	-5.0f,	-5.0f }; //左上
		//v[19].position = { -5.0f,	-5.0f,	5.0f };//左下
		//v[20].position = { -5.0f,	0.0f,	-5.0f };//右上

		//v[21].position = { -5.0f,0.0f,	5.0f };//左下
		//v[22].position = { -5.0f,0.0f,	-5.0f };//右下
		//v[23].position = { -5.0f,-5.0f,	-5.0f };//右上


		float tx = 0.0f;
		float ty = 0.0f;
		float tw = 1.0f;
		float th = 1.0f;

		v[0].texcoord = { tx,		ty, }; //左上
		v[1].texcoord = { tx + tw,	ty, }; //右上
		v[2].texcoord = { tx,		ty +th, }; //左下
		v[3].texcoord = { tx + tw,	ty +th, }; //右上

		v[4].texcoord = { tx	,	ty+th	, };//右下
		v[5].texcoord = { tx +tw,	ty+th	, };//左下
		v[6].texcoord = { tx	,	ty		, };//左下
		v[7].texcoord = { tx +tw,	ty		, };//右下




		//v[4].texcoord = { tx + tw,	ty + th, };//右下
		//v[5].texcoord = { tx,		ty + th, };//左下

		////手前
		//v[6].texcoord = { tx,		ty + th, };//左下
		//v[7].texcoord = { tx + tw,	ty + th, };//右下
		//v[8].texcoord = { tx,		ty, };	   //左下

		//v[9].texcoord =	 { tx+tw,	ty + th, };	   //右上
		//v[10].texcoord = { tx+tw,	ty, }; //右下
		//v[11].texcoord = { tx,		ty, };//左下

		////下面
		//v[12].texcoord = { tx + tw,	ty + th, };//右下
		//v[13].texcoord = { tx + tw,	ty, };//右上
		//v[14].texcoord = { tx,		ty + th, };//左下

		//v[15].texcoord = { tx + tw,	ty, };//右上
		//v[16].texcoord = { tx,		ty, };//左上
		//v[17].texcoord = { tx,		ty + th, };//左下
		DirectXGetDeviceContext()->Unmap(g_VertexBuffer, 0); //バッファのアンマッピング
	}

	//インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DYNAMIC; //バッファの使用法
		bd.ByteWidth = sizeof(unsigned int) * 36; //頂点数
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER; //バッファのバインドフラグ
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPUアクセスフラグ
		DirectXGetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer); //バッファの生成
	}

	//インデックスバッファの設定
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		DirectXGetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned int* index = (unsigned int*)msr.pData;
		//上
		index[0]	= 0;
		index[1]	= 1;
		index[2]	= 2;

		index[3]	= 1;
		index[4]	= 3;
		index[5]	= 2;

		//前
		index[6]	= 2;
		index[7]	= 3;
		index[8]	= 6;

		index[9]	= 3;
		index[10]	= 7;
		index[11]	= 6;

		//右
		index[12]	= 0;
		index[13]	= 2;
		index[14]	= 4;

		index[15]	= 2;
		index[16]	= 6;
		index[17]	= 4;

		//左
		index[18]	= 1;
		index[19]	= 5;
		index[20]	= 3;

		index[21]	= 5;
		index[22]	= 7;
		index[23]	= 3;

		//
		index[24]	= 6;
		index[25]	= 5;
		index[26]	= 4;

		index[27]	= 6;
		index[28]	= 7;
		index[29]	= 5;


		index[30]	= 0;
		index[31]	= 4;
		index[32]	= 1;

		index[33]	= 4;
		index[34]	= 5;
		index[35]	= 1;
		DirectXGetDeviceContext()->Unmap(g_VertexBuffer, 0); //バッファのアンマッピング
	}


	g_Texture = TextureLoad(L"asset\\texture\\title.png");

	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void CubeFinalize(void)
{
	SAFE_RELEASE(g_VertexBuffer); //頂点バッファの解放
}

void CubeUpdate(void)
{
	g_Rotation.y += 0.01f;
}

//ポリゴン描画
void CubeDraw(void)
{


	//シェーダー設定
	Shader_Begin();

	//シェーダー設定マトリクス
	XMMATRIX matrix = XMMatrixIdentity(); //単位行列を作成

	matrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f); //拡大縮小

	matrix *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); //回転

	matrix *= XMMatrixTranslation(0.0, 0.0, 0.0f); //移動
	
	{
		//ビューマトリクス
		matrix *= GetCameraViewMatrix();
		//プロジェクションマトリクス
		matrix *= GetCameraProjectionMatrix();
	}
	Shader_SetMatrix(matrix);//単位行列を設定

	//頂点バッファを設定
	UINT stride1 = sizeof(Vertex); //頂点のサイズ
	UINT offset = 0; //オフセット
	DirectXGetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride1, &offset); //頂点バッファの設定
	
	//インデックスバッファの設定
	DirectXGetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//プリミティブトポロジーの設定
	DirectXGetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //プリミティブトポロジーの設定　トライアングルリスト

	//テクスチャ設定
	ID3D11ShaderResourceView* texture = GetTexture(g_Texture); //テクスチャの取得
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定

	//ポリゴン描画
	DirectXGetDeviceContext()->DrawIndexed(36,0,0); //ポリゴンの描画 画数
}

//トライアングルストリップ
//
//トライアングルリスト
//123 142 
//インデックスバッファ
//012,
//132,
//234,
//軽い、早い、楽


//DirectXGetDeviceContext()->IASetIndexBuffer(g_VertexBuffer, DXGI_FORMAT_R32_Uint, 0); //バッファのマッピング