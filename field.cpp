#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "field.h"

static MODEL* g_Model[3] = {};

struct BLOCK
{
	XMFLOAT3 Position;
	int Type;
};

constexpr int GridMinX = -10;
constexpr int GridMaxX = 10;
constexpr int GridMinZ = 0;
constexpr int GridMaxZ = 20;

constexpr int GridWidth = GridMaxX - GridMinX + 1;     // 21
constexpr int GridHeight = GridMaxZ - GridMinZ + 1;     // 21
constexpr int GridCount = GridWidth * GridHeight;      // 441
constexpr int ItemCount = 59;

static XMFLOAT3 g_Rotation;
static BLOCK g_Block[GridCount];
static BLOCK g_Item[ItemCount] =
{
	{{7.0f,		1.0f,	5.0f},	1},//1
	{{-7.0f,	1.0f,	5.0f},	1},
	{{7.0f,		1.0f,	7.0f},	1},
	{{-7.0f,	1.0f,	7.0f},	1},
	{{7.0f,		1.0f,	10.0f},	1},
	{{-7.0f,	1.0f,	10.0f},	1},
	{{7.0f,		1.0f,	14.0f},	1},
	{{-7.0f,	1.0f,	14.0f},	1},
	{{1.0f,		1.0f,	20.0f},	1},
	{{-1.0f,	1.0f,	20.0f},	1},
	{{0.0f,		2.0f,	10.0f},	1},

	{{10.0f,	1.0f,	1.0f},	0},
	{{10.0f,	1.0f,	2.0f},	0},
	{{10.0f,	1.0f,	3.0f},	0},
	{{10.0f,	1.0f,	4.0f},	0},
	{{10.0f,	1.0f,	5.0f},	0},
	{{10.0f,	1.0f,	6.0f},	0},
	{{10.0f,	1.0f,	7.0f},	0},
	{{10.0f,	1.0f,	8.0f},	0},
	{{10.0f,	1.0f,	9.0f},	0},
	{{10.0f,	1.0f,	10.0f},	0},
	{{10.0f,	1.0f,	11.0f},	0},
	{{10.0f,	1.0f,	12.0f},	0},
	{{10.0f,	1.0f,	13.0f},	0},
	{{10.0f,	1.0f,	14.0f},	0},
	{{10.0f,	1.0f,	15.0f},	0},
	{{10.0f,	1.0f,	16.0f},	0},
	{{10.0f,	1.0f,	17.0f},	0},
	{{10.0f,	1.0f,	18.0f},	0},
	{{10.0f,	1.0f,	19.0f},	0},
	{{10.0f,	1.0f,	20.0f},	0},//31

	{{-10.0f,	1.0f,	0.0f},	0},//32
	{{-10.0f,	1.0f,	1.0f},	0},
	{{-10.0f,	1.0f,	2.0f},	0},
	{{-10.0f,	1.0f,	3.0f},	0},
	{{-10.0f,	1.0f,	4.0f},	0},
	{{-10.0f,	1.0f,	5.0f},	0},
	{{-10.0f,	1.0f,	6.0f},	0},
	{{-10.0f,	1.0f,	7.0f},	0},
	{{-10.0f,	1.0f,	8.0f},	0},
	{{-10.0f,	1.0f,	9.0f},	0},
	{{-10.0f,	1.0f,	10.0f},	0},
	{{-10.0f,	1.0f,	11.0f},	0},
	{{-10.0f,	1.0f,	12.0f},	0},
	{{-10.0f,	1.0f,	13.0f},	0},
	{{-10.0f,	1.0f,	14.0f},	0},
	{{-10.0f,	1.0f,	15.0f},	0},
	{{-10.0f,	1.0f,	16.0f},	0},
	{{-10.0f,	1.0f,	17.0f},	0},
	{{-10.0f,	1.0f,	18.0f},	0},
	{{-10.0f,	1.0f,	19.0f},	0},
	{{-10.0f,	1.0f,	20.0f},	0},

	{{00.0f,	1.0f,	7.0f},	0},
	{{00.0f,	1.0f,	8.0f},	0},
	{{00.0f,	1.0f,	9.0f},	0},
	{{00.0f,	1.0f,	10.0f},	0},
	{{00.0f,	1.0f,	11.0f},	0},
	{{00.0f,	1.0f,	12.0f},	0},//58
//	{{00.0f,	2.0f,	7.0f},	3},
//	{{00.0f,	2.0f,	0.0f},	4},
};



void InitPositions()
{
	int i = 0;
	for (int z = GridMinZ; z <= GridMaxZ; ++z)
	{
		for (int x = GridMinX; x <= GridMaxX; ++x)
		{
			g_Block[i].Position = DirectX::XMFLOAT3((float)x, 0.0f, (float)z);
			g_Block[i].Type = 0;
			++i;
		}
	}
}

void FieldInitialize(void)
{

	g_Model[0] = ModelLoad("asset\\model\\cube.fbx");
	g_Model[1] = ModelLoad("asset\\model\\tree.fbx");
	g_Model[2] = ModelLoad("asset\\model\\ball.fbx");
//	g_Model[3] = ModelLoad("asset\\model\\Bear_Normal.fbx");

	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
}

void FieldFinalize(void)
{
	ModelRelease(g_Model[0]);
	ModelRelease(g_Model[1]);
	ModelRelease(g_Model[2]);
//	ModelRelease(g_Model[3]);

}

void FieldUpdate(void)
{
	//g_Rotation.y += 0.00f;
}

void FieldDraw(void)
{
	for (int i = 0; i < GridCount; i++)
	{
		//頂点シェーダーに変換行列を設定
		MATRIX matrix;

		matrix.World = XMMatrixIdentity();
		matrix.Mtx = XMMatrixIdentity();


		//拡大縮小マトリクス
	//	matrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f); 
		matrix.World *= XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//回転マトリクス
	//	matrix *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); 
		matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z);

		//移動マトリクス
	//	matrix *= XMMatrixTranslation(g_Position.x, g_Position.y, g_Position.z);
		matrix.World *= XMMatrixTranslation(g_Block[i].Position.x, g_Block[i].Position.y, g_Block[i].Position.z);

		matrix.Mtx = matrix.World;

		//	ビューマトリクス
		//	matrix *= GetCameraViewMatrix();
		matrix.Mtx *= GetCameraViewMatrix();

		//プロジェクションマトリクス
	//	matrix *= GetCameraProjectionMatrix();
		matrix.Mtx *= GetCameraProjectionMatrix();

		Shader_SetMatrix(matrix);//シェーダーに行列を設定

		//足場モデル描画
		ModelDraw(g_Model[g_Block[i].Type]);
	}
	for (int i = 0; i < ItemCount; i++)
	{
		//頂点シェーダーに変換行列を設定
		MATRIX matrix;

		matrix.World = XMMatrixIdentity();
		matrix.Mtx = XMMatrixIdentity();


		//拡大縮小マトリクス
	//	matrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f); 
		matrix.World *= XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//回転マトリクス
	//	matrix *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); 
		matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z);

		//移動マトリクス
	//	matrix *= XMMatrixTranslation(g_Position.x, g_Position.y, g_Position.z);
		matrix.World *= XMMatrixTranslation(g_Item[i].Position.x, g_Item[i].Position.y, g_Item[i].Position.z);

		matrix.Mtx = matrix.World;

	//	//	ビューマトリクス
		//	matrix *= GetCameraViewMatrix();
		matrix.Mtx *= GetCameraViewMatrix();

		//プロジェクションマトリクス
	//	matrix *= GetCameraProjectionMatrix();
		matrix.Mtx *= GetCameraProjectionMatrix();

		Shader_SetMatrix(matrix);//シェーダーに行列を設定

		//足場モデル描画
		ModelDraw(g_Model[g_Item[i].Type]);
	}
}
