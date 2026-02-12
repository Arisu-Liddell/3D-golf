#include "directx.h"
#include "camera.h"
#include "keyboard.h"
#include "ball.h"
#include "easing.h"

static XMMATRIX g_ViewMatrix;
static XMMATRIX g_ProjectionMatrix;

static XMFLOAT3 g_Position;
static XMFLOAT3 g_Rotation;
static XMFLOAT3 g_Target;

//static XMFLOAT3 g_FixCamera[4] =
//{
//	{  0.0f, 10.0f, -10.0f }, // 0 : 前
//	{ 10.0f, 10.0f,  0.0f }, // 1 : 右
//	{  0.0f, 10.0f,  10.0f }, // 2 : 後
//	{-10.0f, 10.0f,  0.0f }, // 3 : 左
//};

static int g_FixCameraIndex;
static float g_FixCameraTime;
static XMFLOAT3 g_FixCameraOld;
static float g_ShakeTime;
static float g_ShakePower;
static const float CAMERA_HEIGHT = 2.0f;


XMMATRIX GetCameraViewMatrix(void)
{
	return g_ViewMatrix;
}
XMMATRIX GetCameraProjectionMatrix(void)
{
	return g_ProjectionMatrix;
}
///jugyou delog
//カメラ向き
XMFLOAT3 GetCameraForward(void)
{
	XMFLOAT3 forward;

	forward.x = g_Target.x - g_Position.x;
	forward.y = g_Target.y - g_Position.y;
	forward.z = g_Target.z - g_Position.z;

	return forward;
}

XMFLOAT3 GetCameraPosition(void)
{
	return g_Position;
}

void CameraInitialize(void)//ポリゴン初期化
{
	g_Position = XMFLOAT3(0.0f, 10.0f, -10.0f);
	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Target = XMFLOAT3(0.0f, 0.0f, 0.0f);

	g_ShakeTime = 0.0f;
	g_ShakePower = 0.0f;

	g_FixCameraIndex = 0;
	g_FixCameraTime = 0.0f;
	//g_FixCameraOld = g_FixCamera[0];

	// ★ position は target + offset
	//g_Position = XMFLOAT3(
	//	g_Target.x + g_FixCamera[0].x,
	//	g_Target.y + g_FixCamera[0].y,
	//	g_Target.z + g_FixCamera[0].z
	//);
}
void CameraFinalize(void)//ポリゴン終了
{

}
void CameraUpdate(void)//ポリゴン更新
{
	XMFLOAT3 BallTarget = GetBallPosition();

	g_Target.x += (BallTarget.x - g_Target.x) * 0.1f;
	g_Target.y += (BallTarget.y - g_Target.y) * 0.1f;
	g_Target.z += (BallTarget.z - g_Target.z) * 0.1f;


	if (Keyboard_IsKeyDown(KK_RIGHT))
	{
		g_Rotation.y -= 0.1f;
	}
	if (Keyboard_IsKeyDown(KK_LEFT))
	{
		g_Rotation.y += 0.1f;
	}
	if(Keyboard_IsKeyDown(KK_UP))
	{
		g_Rotation.x -= 0.1f;
	}
	if (Keyboard_IsKeyDown(KK_DOWN))
	{
		g_Rotation.x += 0.1f;
	}

	const float CAMERA_DISTANCE = 3.0f;

	// ピッチ制限（真上・真下を向きすぎないように）
	const float PITCH_LIMIT = DirectX::XM_PIDIV2 - 0.1f;
	if (g_Rotation.x > PITCH_LIMIT) g_Rotation.x = PITCH_LIMIT;
	if (g_Rotation.x < -PITCH_LIMIT) g_Rotation.x = -PITCH_LIMIT;

	//球面座標計算メモ
	//g_Position.x = g_Target.x + sinf(g_Rotation.y) * 3.0f;
	//g_Position.z = g_Target.z - cosf(g_Rotation.y) * 3.0f;
	//sin cosを逆にすると90.逆になる　横基準ではなく奥行き基準で

	//カメラの移動時間
	g_FixCameraTime += 1.0f / 60.0f;
	if (g_FixCameraTime > 1.0f)
	{
		g_FixCameraTime = 1.0f;
	}

	////カメラ切り替え
	//if (Keyboard_IsKeyTrigger(KK_ADD))
	//{
	//	g_FixCameraIndex--;
	//	if (g_FixCameraIndex < 0)
	//	{
	//		g_FixCameraIndex = 3;
	//	}
	//	g_FixCameraTime = 0.0f;
	//	g_FixCameraOld = XMFLOAT3
	//	(
	//		g_Position.x - g_Target.x,
	//		g_Position.y - g_Target.y,
	//		g_Position.z - g_Target.z
	//	);
	//}
	//if (Keyboard_IsKeyTrigger(KK_SUBTRACT))
	//{
	//	g_FixCameraIndex++;
	//	if (g_FixCameraIndex > 3)
	//	{
	//		g_FixCameraIndex = 0;
	//	}
	//	g_FixCameraTime = 0.0f;
	//	g_FixCameraOld = XMFLOAT3(
	//		g_Position.x - g_Target.x,
	//		g_Position.y - g_Target.y,
	//		g_Position.z - g_Target.z
	//	);
	//}

	////カメラ位置線形補間
	//float ease = easeInOutCubic(g_FixCameraTime);
	//// ease を補間係数として使用
	//g_Position.x = g_FixCameraOld.x * (1.0f - ease) + g_FixCamera[g_FixCameraIndex].x * ease;
	//g_Position.y = g_FixCameraOld.y * (1.0f - ease) + g_FixCamera[g_FixCameraIndex].y * ease;
	//g_Position.z = g_FixCameraOld.z * (1.0f - ease) + g_FixCamera[g_FixCameraIndex].z * ease;

	//カメラシェイク処理
	g_ShakeTime += 1.0f / 60.0f;
	g_Position.y += sinf(g_ShakeTime * 120.0f) * 1.0f * g_ShakePower;

	//1フレームの時間
	const float dt = 1.0f / 60.0f;

	// --- 球面座標 → 直交座標（ここは今のままでOK） ---
	g_Position.x = g_Target.x + cosf(g_Rotation.x) * sinf(g_Rotation.y) * CAMERA_DISTANCE;
	g_Position.y = g_Target.y + sinf(g_Rotation.x) * CAMERA_DISTANCE;
	g_Position.z = g_Target.z - cosf(g_Rotation.x) * cosf(g_Rotation.y) * CAMERA_DISTANCE;

	// （任意）欲しいなら “基準高さ” を足す：上下視点を残したまま全体を持ち上げる
	g_Position.y += CAMERA_HEIGHT;

	// --- Shake（最後に1回だけ） ---
	g_ShakeTime += dt;

	if (g_ShakePower > 0.0f)
	{
		g_Position.y += sinf(g_ShakeTime * 120.0f) * 0.1f * g_ShakePower;
		g_ShakePower -= 0.1f;
		if (g_ShakePower < 0.0f) g_ShakePower = 0.0f;
	}
	//g_Position.y = g_Target.y + CAMERA_HEIGHT;
	//// Shake
	//g_ShakeTime += dt;
	//if (g_ShakePower > 0.0f)
	//{
	//	g_Position.y += sinf(g_ShakeTime * 120.0f) * 0.1f * g_ShakePower;
	//}
	//g_ShakePower -= 0.1f;//徐々に減衰
	//if (g_ShakePower < 0.0f)
	//{
	//	g_ShakePower = 0.0f;
	//}
}
void CameraDraw(void)//ポリゴン描画
{
	//ビューマトリクス(カメラ)

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	g_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&g_Position), XMLoadFloat3(&g_Target), XMLoadFloat3(&up));

	//プロジェクションマトリクス
	g_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);
}
void CameraDrawMap(void)//ポリゴン描画
{
	//ビューマトリクス(カメラ)
	XMFLOAT3 position = XMFLOAT3(GetBallPosition());
	position.y = 0.0f;
	position.y += 15.0f;
	XMFLOAT3 target = XMFLOAT3(GetBallPosition());
	XMFLOAT3 up = XMFLOAT3(0.0f, 0.0f, 1.0f);
	g_ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//プロジェクションマトリクス
	g_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, 256.0f/256.0f, 1.0f, 1000.0f);
}

//g_Target 
void CameraShake(float Tremor)
{
	if (Tremor > g_ShakePower)
	{
		g_ShakePower = Tremor;
	}
	g_ShakeTime = 0.0f;
}
