#include "directx.h"
#include "camera.h"
#include "keyboard.h"


static XMMATRIX g_ViewMatrix;
static XMMATRIX g_ProjectionMatrix;
static XMFLOAT3 g_Position;
static XMFLOAT3 g_Rotation;
static XMFLOAT3 g_Target;

XMMATRIX GetCameraViewMatrix(void)
{
	return g_ViewMatrix;
}
XMMATRIX GetCameraProjectionMatrix(void)
{
	return g_ProjectionMatrix;
}

void CameraInitialize(void)//ポリゴン初期化
{
	g_Position = XMFLOAT3(0.0f, 10.0f,	-10.0f);
	g_Rotation = XMFLOAT3(0.0f,	0.0f,	0.0f);
	g_Target   = XMFLOAT3(0.0f, 0.0f,	0.0f);
}
void CameraFinalize(void)//ポリゴン終了
{

}
void CameraUpdate(void)//ポリゴン更新
{
	if (Keyboard_IsKeyDown(KK_A))
	{
		g_Rotation.y += 0.1f;
	}
	if (Keyboard_IsKeyDown(KK_D))
	{
		g_Rotation.y -= 0.1f;
	}

	g_Position.x = sinf(g_Rotation.y) * 3.0f;
	g_Position.z = -cosf(g_Rotation.y) * 3.0f;
	//sin cosを逆にすると90.逆になる　横基準ではなく奥行き基準で

	if (Keyboard_IsKeyDown(KK_W))
	{
		g_Target.y += 1.0f;
	}
	if (Keyboard_IsKeyDown(KK_S))
	{
		g_Target.y -= 1.0f;
	}
}
void CameraDraw(void)//ポリゴン描画
{
	//ビューマトリクス(カメラ)
	
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	g_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&g_Position), XMLoadFloat3(&g_Target), XMLoadFloat3(&up));

	//プロジェクションマトリクス
	g_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);
}