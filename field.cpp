#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "field.h"


static MODEL* g_Model[5] = {};
static XMFLOAT3 g_Rotation;


BLOCK* GetFieldBlock(void)
{
	return g_Block1;
}
BLOCK* GetFieldItem(void)
{
	return g_Block2;
}

void InitPositions()
{
	int i = 0;
	for (int z = GridMinZ; z <= GridMaxZ; ++z)
	{
		for (int x = GridMinX; x <= GridMaxX; ++x)
		{
			g_Block1[i].Position = DirectX::XMFLOAT3((float)x, 0.0f, (float)z);
			g_Block1[i].Type = 0;
			++i;
		}
	}
}

void FieldInitialize(void)
{

	g_Model[0] = ModelLoad("asset\\model\\cube.fbx");
	g_Model[1] = ModelLoad("asset\\model\\tree.fbx");
	g_Model[2] = ModelLoad("asset\\model\\Goal.fbx");
	g_Model[3] = ModelLoad("asset\\model\\ball.fbx");
	g_Model[4] = ModelLoad("asset\\model\\Bear_Normal.fbx");

	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
}

void FieldFinalize(void)
{
	ModelRelease(g_Model[0]);
	ModelRelease(g_Model[1]);
	ModelRelease(g_Model[2]);
	ModelRelease(g_Model[3]);
	ModelRelease(g_Model[4]);
}

void FieldUpdate(void)
{
}

void FieldDraw(void)
{
	InitPositions();
	for (int i = 0; i < GridCount; i++)
	{
		//頂点シェーダーに変換行列を設定
		MATRIX matrix;

		matrix.World = XMMatrixIdentity();
		matrix.Mtx = XMMatrixIdentity();


		//拡大縮小マトリクス
		matrix.World *= XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//回転マトリクス
		matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z);

		//移動マトリクス
		matrix.World *= XMMatrixTranslation(g_Block1[i].Position.x, g_Block1[i].Position.y, g_Block1[i].Position.z);
		matrix.Mtx = matrix.World;

		//	ビューマトリクス
		matrix.Mtx *= GetCameraViewMatrix();

		//プロジェクションマトリクス
		matrix.Mtx *= GetCameraProjectionMatrix();

		Shader_SetMatrix(matrix);//シェーダーに行列を設定

		//足場モデル描画
		ModelDraw(g_Model[g_Block1[i].Type]);
	}

	for (int i = 0; i < Grid2Count; i++)
	{
		//頂点シェーダーに変換行列を設定
		MATRIX matrix;

		matrix.World = XMMatrixIdentity();
		matrix.Mtx = XMMatrixIdentity();


		//拡大縮小マトリクス
		matrix.World *= XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//回転マトリクス
		matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z);

		//移動マトリクス
		matrix.World *= XMMatrixTranslation(g_Block2[i].Position.x, g_Block2[i].Position.y, g_Block2[i].Position.z);

		matrix.Mtx = matrix.World;

		//ビューマトリクス
		matrix.Mtx *= GetCameraViewMatrix();

		//プロジェクションマトリクス
		matrix.Mtx *= GetCameraProjectionMatrix();

		Shader_SetMatrix(matrix);//シェーダーに行列を設定

		//足場モデル描画
		ModelDraw(g_Model[g_Block2[i].Type]);
	}
}
