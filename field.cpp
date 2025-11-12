#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "field.h"

static MODEL* g_Model[2] = {};

struct BLOCK
{
	XMFLOAT3 Position;
	int Type;
};

static BLOCK g_Block[5] =
{
	{{0.0f,0.0f,0.0f},0},
	{{1.0f,0.0f,0.0f},0},
	{{2.0f,0.0f,0.0f},1},
	{{3.0f,0.0f,0.0f},0},
	{{4.0f,0.0f,0.0f},0},
};
static XMFLOAT3 g_Rotation;


void FieldInitialize(void)
{
	g_Model[0] = ModelLoad("asset\\model\\cube.fbx");
	g_Model[1] = ModelLoad("asset\\model\\tree.fbx");

	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
}

void FieldFinalize(void)
{
	ModelRelease(g_Model[0]);
	ModelRelease(g_Model[1]);
}

void FieldUpdate(void)
{
	//g_Rotation.y += 0.00f;
}

void FieldDraw(void)
{
	for (int i = 0; i < 5; i++)
	{
		//頂点シェーダーに変換行列を設定
		MATRIX matrix;

		matrix.World = XMMatrixIdentity();
		matrix.Matrix = XMMatrixIdentity();


		//拡大縮小マトリクス
	//	matrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f); 
		matrix.World *= XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//回転マトリクス
	//	matrix *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); 
		matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z);

		//移動マトリクス
	//	matrix *= XMMatrixTranslation(g_Position.x, g_Position.y, g_Position.z);
		matrix.World *= XMMatrixTranslation(g_Block[i].Position.x, g_Block[i].Position.y, g_Block[i].Position.z);

		matrix.Matrix = matrix.World;

		//	ビューマトリクス
		//	matrix *= GetCameraViewMatrix();
		matrix.Matrix *= GetCameraViewMatrix();

		//プロジェクションマトリクス
	//	matrix *= GetCameraProjectionMatrix();
		matrix.Matrix *= GetCameraProjectionMatrix();


		Shader_SetMatrix(matrix);//シェーダーに行列を設定
		//モデル描画
		ModelDraw(g_Model[g_Block[i].Type]);
	}
}
