#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "goal.h"
#include "field.h"

static MODEL* g_Model = NULL;

static XMFLOAT3 g_Position;//位置
static XMFLOAT3 g_Rotation;//回転



XMFLOAT3 GetGoalPosition(void)
{
	return g_Position;
}

void GoalInitialize(void)
{
	g_Model = ModelLoad("asset\\model\\goal.fbx");
	g_Position = XMFLOAT3(0.0f, 2.0f, 8.0f);//位置初期化
	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
}

void GoalFinalize(void)
{
	ModelRelease(g_Model);

}

void GoalUpdate(void)
{

}

void GoalDraw(void)
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
	matrix.World *= XMMatrixTranslation(g_Position.x, g_Position.y, g_Position.z);

	matrix.Mtx = matrix.World;

	//	ビューマトリクス
	matrix.Mtx *= GetCameraViewMatrix();

	//プロジェクションマトリクス
	matrix.Mtx *= GetCameraProjectionMatrix();

	Shader_SetMatrix(matrix);//シェーダーに行列を設定
	//モデル描画
	ModelDraw(g_Model);
}
