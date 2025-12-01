#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "field.h"


static MODEL* g_Model[3] = {};
static XMFLOAT3 g_Rotation;


BLOCK* GetFieldBlock(void)
{
	return g_Block;
}
BLOCK* GetFieldItem(void)
{
	return g_Item;
}


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
	g_Model[2] = ModelLoad("asset\\model\\Goal.fbx");
	//g_Model[3] = ModelLoad("asset\\model\\Goal.fbx");

	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
}

void FieldFinalize(void)
{
	ModelRelease(g_Model[0]);
	ModelRelease(g_Model[1]);
	ModelRelease(g_Model[2]);
	//ModelRelease(g_Model[3]);

}

void FieldUpdate(void)
{
	//g_Rotation.y += 0.00f;
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
