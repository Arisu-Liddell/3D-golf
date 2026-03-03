#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "ball.h"
#include "field.h"
#include "goal.h"
#include "main.h"
#include "score.h"
#include "ranking.h"
#include "effect.h"
#include "trail.h"
#include "Shadow.h"
#include "sound.h"

enum BALL_STATE
{
	BALL_STATE_MOVE,
	BALL_STATE_GOAL,
	BALL_STATE_NONE
};
enum GROUND_STATE
{
	GROUND_STATE_GROUND,
	GROUND_STATE_FLOW
};
static BALL_STATE g_State;
static int g_StateCount;

static MODEL* g_Model = NULL;
static XMFLOAT3 g_Position;//位置
static XMFLOAT3 g_Rotation;//回転
static XMFLOAT3 g_Velocity;//速度

static float dt = 1.0f / 60.0f;
static bool g_OnGroundA;
static bool g_OnGroundB;
static int g_JumpSE = -1; // ジャンプSE

void BallHitCheck(void);
void BallMove(void);//ポリゴン移動処理


XMFLOAT3 GetBallPosition(void)
{
	return g_Position;
}

void BallInitialize(void)
{
	g_Model = ModelLoad("asset\\model\\ball.fbx");
	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
	g_Position = XMFLOAT3(0.0f, 3.0f, 0.0f);//位置初期化
	g_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);//速度初期化
	g_OnGroundA = false;
	g_OnGroundB = false;
	g_State  = BALL_STATE_MOVE;
	g_StateCount = 0;
	if (Sound::GetInstance())
		g_JumpSE = Sound::GetInstance()->LoadSound("asset\\sound\\Jump.wav");

	ResetTrailPosition(g_Position);
}

void BallFinalize(void)
{
	ModelRelease(g_Model);
}

void BallUpdate(void)
{
	//ステートマシン
	switch (g_State)
	{
	//バウンド
	case BALL_STATE_MOVE:
		BallMove();
		break;
	case BALL_STATE_GOAL:
		g_StateCount++;
		//リザルト画面遷移	
		if (g_StateCount > 30)
		{
			g_StateCount = 0;
			g_State = BALL_STATE_NONE;
			Transition(SCENE_RESULT);
		}
		break;
	default:
		break;
	}

}
void BallMove(void)
{
	XMFLOAT3 cameraforward = GetCameraForward();//これ単体だと距離で回転速度が変わってしまう
	XMFLOAT3 force = { 0.0f,0.0f,0.0f };//力ベクトル初期化

	cameraforward.y = 0.0f;//y成分を0にする
	//ベクトルの長さ
	float length = sqrtf(cameraforward.x * cameraforward.x
		+ cameraforward.y * cameraforward.y
		+ cameraforward.z * cameraforward.z);
	//正規化
	cameraforward.x /= length;
	cameraforward.y /= length;
	cameraforward.z /= length;

	if (g_OnGroundA == true or g_OnGroundB == true)
	{
		//移動入力
		if (Keyboard_IsKeyDown(KK_A))
		{
			force.x -= cameraforward.z;
			force.z += cameraforward.x;
		}
		if (Keyboard_IsKeyDown(KK_D))
		{
			force.x += cameraforward.z;
			force.z -= cameraforward.x;
		}
		if (Keyboard_IsKeyDown(KK_W))
		{
			force.x += cameraforward.x;
			force.z += cameraforward.z;
		}
		if (Keyboard_IsKeyDown(KK_S))
		{
			force.x -= cameraforward.x;
			force.z -= cameraforward.z;
		}
	}
	if (Keyboard_IsKeyTrigger(KK_SPACE))
	{
		if (g_OnGroundA == true or g_OnGroundB == true)
		{
			CameraShake(2.0f);//カメラシェイク
			//g_Velocity.x += cameraforward.x * 5.0f;
			//g_Velocity.z += cameraforward.z * 5.0f;
			//ジャンプ
			g_Velocity.y += 7.0f;

			CreateEffect(g_Position);
			if (Sound::GetInstance() && g_JumpSE >= 0)
				Sound::GetInstance()->PlaySound(g_JumpSE, 0);

			ScoreAdd(1);//スコア加算
		}
	}

	//ベクトルの長さ
	float forcelength = sqrtf(force.x * force.x
		+ force.y * force.y
		+ force.z * force.z);
	//正規化
	if (forcelength > 1.0f)
	{
		force.x /= forcelength;
		force.y /= forcelength;
		force.z /= forcelength;
	}
	//力を速度に変換
	g_Velocity.x += force.x * 10.0f * dt;
	g_Velocity.z += force.z * 10.0f * dt;

	//重力
	g_Velocity.y -= 9.8f * dt;

	//摩擦]
	if (g_OnGroundA == true or g_OnGroundB == true)
	{
		if (g_OnGroundB == true)
		{
			g_Velocity.x -= g_Velocity.x * 0.1f * dt;
			g_Velocity.z -= g_Velocity.z * 0.1f * dt;
			OutputDebugStringA("OnGroundB!!!");
		}
		else
		{
			g_Velocity.x -= g_Velocity.x * dt;
			g_Velocity.z -= g_Velocity.z * dt;
		}
	}
	else if (g_OnGroundA == false or g_OnGroundB == false)
	{
		g_Velocity.x -= g_Velocity.x * 0.3f * dt;
		g_Velocity.z -= g_Velocity.z * 0.3f * dt;
	}
	g_Velocity.y -= g_Velocity.y * 0.1f * dt;
	//移動
	g_Position.x += g_Velocity.x * dt;
	g_Position.z += g_Velocity.z * dt;
	g_Position.y += g_Velocity.y * dt;

	//当たり判定
	BallHitCheck();

	//トレイル座標設定
	SetTrailPosition(g_Position);

	//影座標設定
	SetShadowPosition(g_Position);

	//ゴール衝突判定
	XMFLOAT3 gorlPosition = GetGoalPosition();
	XMFLOAT3 direction;
	direction.x = gorlPosition.x - g_Position.x;
	direction.y = gorlPosition.y - g_Position.y;
	direction.z = gorlPosition.z - g_Position.z;

	//ベクトルの長さ
	float GoalLength = sqrtf(direction.x * direction.x
		+ direction.y * direction.y
		+ direction.z * direction.z);
	if (GoalLength < 1.0f)
	{
		CreateEffect(gorlPosition);
		//ランキング登録
		SetRanking(GetScore());

		g_State = BALL_STATE_GOAL;
	}
	//リスポーン
	if (g_Position.y < -30.0)
	{
		g_Position = XMFLOAT3(0.0f, 3.0f, 0.0f);//位置初期化
		g_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);//速度初期化
	}
}

void BallDraw(void)
{
	//頂点シェーダーに変換行列を設定
	MATRIX matrix;

	matrix.World = XMMatrixIdentity(); 
	matrix.Mtx = XMMatrixIdentity(); 
	//Mtx = Matrix ?

	//拡大縮小マトリクス
	matrix.World *= XMMatrixScaling(1.0f, 1.0f, 1.0f); 

	//回転マトリクス
//	matrix *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); 
	matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z); 

	//移動マトリクス
//	matrix *= XMMatrixTranslation(g_Position.x, g_Position.y, g_Position.z);
	matrix.World *= XMMatrixTranslation(g_Position.x, g_Position.y, g_Position.z);

	matrix.Mtx = matrix.World;

//	ビューマトリクス
//	matrix *= GetCameraViewMatrix();
	matrix.Mtx *= GetCameraViewMatrix();

	//プロジェクションマトリクス
//	matrix *= GetCameraProjectionMatrix();
	matrix.Mtx *= GetCameraProjectionMatrix();


	Shader_SetMatrix(matrix);//シェーダーに行列を設定
	//モデル描画
	ModelDraw(g_Model);
}

// block / item 共通の衝突処理（g_State ゲートは入れない）
static void ResolveBallVsBlocks(
	BLOCK* blocks, int count,
	float bounceY,
	bool& onGroundFlag)           // block:0.7f / item:0.3f
{
	const float collitionRadius = 0.5f;
	const float ballRadius = 0.2f;
	bool landed = false;


	for (int i = 0; i < count; i++)
	{
		float blockTop = blocks[i].Position.y + collitionRadius;

		if (blocks[i].Type == 5)
			blockTop = blocks[i].Position.y + std::min((g_Position.z - blocks[i].Position.z), collitionRadius);
		else if (blocks[i].Type == 6)
			blockTop = blocks[i].Position.y + std::min(-(g_Position.x - blocks[i].Position.x), collitionRadius);
		else if (blocks[i].Type == 7)
			blockTop = blocks[i].Position.y + std::min(-(g_Position.z - blocks[i].Position.z), collitionRadius);
		else if (blocks[i].Type == 8)
			blockTop = blocks[i].Position.y + std::min((g_Position.x - blocks[i].Position.x), collitionRadius);


		// =========================
		// X/Z方向（Yが範囲内のとき）
		// =========================
		if (blocks[i].Position.y - collitionRadius < g_Position.y &&
			g_Position.y < blockTop - 0.1f)
		{
			// X方向（Zが範囲内）
			if (blocks[i].Position.z - collitionRadius < g_Position.z &&
				g_Position.z < blocks[i].Position.z + collitionRadius)
			{
				if (blocks[i].Position.x - collitionRadius < g_Position.x + ballRadius &&
					g_Position.x - ballRadius < blocks[i].Position.x + collitionRadius)
				{
					if (blocks[i].Position.x < g_Position.x)
					{
						g_Position.x = blocks[i].Position.x + collitionRadius + ballRadius;
						CameraShake(4.0f);//カメラシェイク
						CreateEffect(g_Position);
					}
					else
					{
						g_Position.x = blocks[i].Position.x - collitionRadius - ballRadius;
						CameraShake(4.0f);//カメラシェイク
						CreateEffect(g_Position);
					}
					g_Velocity.x *= -0.5f;
				}
			}
			// Z方向（Xが範囲内）
			else if (blocks[i].Position.x - collitionRadius < g_Position.x &&
				g_Position.x < blocks[i].Position.x + collitionRadius)
			{
				if (blocks[i].Position.z - collitionRadius < g_Position.z + ballRadius &&
					g_Position.z - ballRadius < blocks[i].Position.z + collitionRadius)
				{
					if (blocks[i].Position.z < g_Position.z)
					{
						g_Position.z = blocks[i].Position.z + collitionRadius + ballRadius;
						CameraShake(4.0f);//カメラシェイク
						CreateEffect(g_Position);
					}
					else
					{
						g_Position.z = blocks[i].Position.z - collitionRadius - ballRadius;
						CameraShake(4.0f);//カメラシェイク
						CreateEffect(g_Position);
					}
					g_Velocity.z *= -0.5f;
				}
			}
		}
		// =========================
		// Y方向（X/Zが範囲内のとき）
		// =========================
		else
		{
			if (blocks[i].Position.z - collitionRadius < g_Position.z &&
				g_Position.z < blocks[i].Position.z + collitionRadius)
			{
				if (blocks[i].Position.x - collitionRadius < g_Position.x &&
					g_Position.x < blocks[i].Position.x + collitionRadius)
				{
					if (blocks[i].Position.y - collitionRadius < g_Position.y + ballRadius &&
						g_Position.y - ballRadius < blockTop)
					{
						if (blockTop < g_Position.y/* - blocks[i].Position.y < g_Position.y*/)
						{
							// 上から衝突
							g_Position.y = blockTop + ballRadius;

							// “初回着地だけ”エフェクト
							if (onGroundFlag == false)
							{
								if (g_Velocity.y < -3.0f)
								{
									//CameraShake((g_Velocity.y mod 10)+ 2.0f);//カメラシェイク
									CameraShake(
										fminf(fmodf(fabsf(g_Velocity.y), 1.0f) + 2.0, 0.5f)
									);
									CreateEffect(g_Position);
									onGroundFlag = false;
								}
								else
								{
									CameraShake(
										fminf(fmodf(fabsf(g_Velocity.y), 1.0f), 0.5f)
									);
									onGroundFlag = true;
								}
							}
							float dt = 1.0f / 60.0f;

							if (blocks[i].Type == 5)
							{
								g_Velocity.z += -1.0f * dt;
								g_Velocity.z += ( - g_Velocity.y * -bounceY)*1.05;
								g_Velocity.y = 0.0f;
							}
							else if (blocks[i].Type == 6)
							{
								g_Velocity.x += 1.0f * dt;
								g_Velocity.x += (g_Velocity.y * -bounceY)*1.05;
								g_Velocity.y = 0.0f;
							}
							else if (blocks[i].Type == 7)
							{
								g_Velocity.z += 1.0f * dt;
								g_Velocity.z += (g_Velocity.y * -bounceY)*1.05;
								g_Velocity.y = 0.0f;
							}
							else if (blocks[i].Type == 8)
							{
								g_Velocity.x += -1.0f * dt;
								g_Velocity.x += ( - g_Velocity.y * -bounceY)*1.05;
								g_Velocity.y = 0.0f;
							}
							else
							{
								g_Velocity.y *= -bounceY;
							}
						}
						else
						{
							// 下から衝突
							g_Position.y = blocks[i].Position.y - collitionRadius - ballRadius;
							g_Velocity.y *= -bounceY;
						}
					}
					else
					{
						// 当たってない
						onGroundFlag = false;
					}
				}
			}
		}
	}
};

void BallHitCheck(void)
{
	BLOCK* block = GetFieldBlock();
	BLOCK* item = GetFieldItem();

	ResolveBallVsBlocks(block, GridCount, 0.7f, g_OnGroundA);
	ResolveBallVsBlocks(item, GetFieldItemCount(), 0.3f, g_OnGroundB);
}
