#include "directx.h"
#include "camera.h"
#include "keyboard.h"


XMMATRIX g_ViewMatrix;
XMMATRIX g_ProjectionMatrix;

XMFLOAT3 g_CameraPosition;

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
	g_CameraPosition = XMFLOAT3(0.0f, 10.0f, -10.0f);
}
void CameraFinalize(void)//ポリゴン終了
{

}
void CameraUpdate(void)//ポリゴン更新
{
	if (Keyboard_IsKeyDown(KK_A))
	{
		g_CameraPosition.x += 1.0f;
	}
	if (Keyboard_IsKeyDown(KK_D))
	{
		g_CameraPosition.x -= 1.0f;	
	}
	if (Keyboard_IsKeyDown(KK_W))
	{
		g_CameraPosition.y += 1.0f;
	}
	if (Keyboard_IsKeyDown(KK_S))
	{
		g_CameraPosition.y -= 1.0f;
	}
}
void CameraDraw(void)//ポリゴン描画
{
	//ビューマトリクス(カメラ)
	XMFLOAT3 target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	g_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&g_CameraPosition), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//プロジェクションマトリクス
	g_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);
}