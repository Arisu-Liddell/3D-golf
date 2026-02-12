#include "directx.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "SkyDome.h"
#include "goal.h"
#include "main.h"
#include "Shadow.h"



static MODEL* g_Model = NULL;
static XMFLOAT3 g_Position;//位置
static XMFLOAT3 g_Rotation;//回転

void SkyDomeInitialize(void)
{
	g_Model = ModelLoad("asset\\model\\sky.fbx");
	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
	g_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);//位置初期化
}

void SkyDomeFinalize(void)
{
	ModelRelease(g_Model);
}

void SkyDomeUpdate(void)
{

}

void SkyDomeDraw(void)
{
	//頂点シェーダーに変換行列を設定
	MATRIX matrix;

	matrix.World = XMMatrixIdentity();
	matrix.Mtx = XMMatrixIdentity();

	//拡大縮小マトリクス
	matrix.World *= XMMatrixScaling(100.0f, 100.0f, 100.0f);

	//回転マトリクス
	matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z);

	matrix.Mtx = matrix.World;

	//	ビューマトリクス
	matrix.Mtx *= GetCameraViewMatrix();

	//プロジェクションマトリクス
	matrix.Mtx *= GetCameraProjectionMatrix();

	Shader_SetMatrix(matrix);//シェーダーに行列を設定
	//モデル描画
	ModelDraw(g_Model);
}
