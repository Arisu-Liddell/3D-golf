#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "ball.h"

static MODEL* g_Model = NULL;

static XMFLOAT3 g_Position;
static XMFLOAT3 g_Rotation;


void BallInitialize(void)
{
	g_Model = ModelLoad("asset\\model\\ball.fbx");
	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
	g_Position = XMFLOAT3(0.0f, 3.0f, 0.0f);//位置初期化
}

void BallFinalize(void)
{
	ModelRelease(g_Model);

}

void BallUpdate(void)
{

}

void BallDraw(void)
{
	XMMATRIX matrix = XMMatrixIdentity(); //単位行列を作成
	matrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f); //拡大縮小マトリクス
	matrix *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); //回転マトリクス
	matrix *= XMMatrixTranslation(g_Position.x, g_Position.y, g_Position.z); //移動マトリクス
	matrix *= GetCameraViewMatrix();//ビューマトリクス
	matrix *= GetCameraProjectionMatrix();//プロジェクションマトリクス
	Shader_SetMatrix(matrix);//シェーダーに行列を設定
	//モデル描画
	ModelDraw(g_Model);
}
