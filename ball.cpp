#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "ball.h"
#include "field.h"
#include "goal.h"

static MODEL* g_Model = NULL;

static XMFLOAT3 g_Position;//位置
static XMFLOAT3 g_Rotation;//回転
static XMFLOAT3 g_Velocity;//速度
static float dt = 1.0f / 60.0f;

void BallHitCheck(void);

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
}

void BallFinalize(void)
{
	ModelRelease(g_Model);

}

void BallUpdate(void)
{
	//XMFLOAT3 cameraforward = GetCameraForward();//これ単体だと距離で回転速度が変わってしまう

	//cameraforward.y = 0.0f;
	//XMFLOAT3 force = { 0.0f,0.0f,0.0f };

	////ベクトルの長さ
	//float length = sqrtf(cameraforward.x * cameraforward.x
	//	+ cameraforward.y * cameraforward.y
	//	+ cameraforward.z * cameraforward.z);

	////正規化
	//cameraforward.x /= length;
	//cameraforward.y /= length;
	//cameraforward.z /= length;

	//XMFLOAT3 force = {0.0f,0.0f,0.0f};

	//if (Keyboard_IsKeyDown(KK_A))
	//{
	//	g_Velocity.x -= 10.0f * dt;
	//}
	//if (Keyboard_IsKeyDown(KK_D))
	//{
	//	g_Velocity.x += 10.0f * dt;
	//}
	//if (Keyboard_IsKeyDown(KK_W))
	//{
	//	g_Velocity.z += 10.0f * dt;
	//}
	//if (Keyboard_IsKeyDown(KK_S))
	//{
	//	g_Velocity.z -= 10.0f * dt;
	//}
	

	//ベクトルの長さ
	//float forcelength = sqrtf(force.x * force.x
	//	+ force.y * force.y
	//	+ force.z * force.z);

	//if (forcelength > 1.0f)
	//{
	//	//正規化
	//	force.x /= forcelength;
	//	force.y /= forcelength;
	//	force.z /= forcelength;
	//}

	//重力
	g_Velocity.y -= 9.8f * dt;
	//摩擦
	g_Velocity.x -= g_Velocity.x * 2.0f * dt;
	g_Velocity.z -= g_Velocity.z * 2.0f * dt;
	g_Velocity.y -= g_Velocity.y * 0.1f * dt;

	//移動
	g_Position.x += g_Velocity.x * dt;
	g_Position.z += g_Velocity.z * dt;
	g_Position.y += g_Velocity.y * dt;

	//当たり判定
	BallHitCheck();

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
		//リザルト画面遷移
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
//	matrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f); 
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

void BallHitCheck(void)
{
	XMFLOAT3 cameraforward = GetCameraForward();//これ単体だと距離で回転速度が変わってしまう
	cameraforward.y = 0.0f;
	//ベクトルの長さ
	float length = sqrtf(cameraforward.x * cameraforward.x
		+ cameraforward.y * cameraforward.y
		+ cameraforward.z * cameraforward.z);

	//正規化
	cameraforward.x /= length;
	cameraforward.y /= length;
	cameraforward.z /= length;

	BLOCK* block = GetFieldBlock();
	BLOCK* item = GetFieldItem();

	float collitionRadius = 0.5f;
	float ballRadius = 0.2f;
	float e = 0.5f;

	for (int i = 0; i < GridCount; i++)
	{
		if (block[i].Position.y - collitionRadius < g_Position.y &&
			g_Position.y < block[i].Position.y + collitionRadius)
		{
			//横方向
			if (block[i].Position.z - collitionRadius < g_Position.z &&
				g_Position.z < block[i].Position.z + collitionRadius)
			{
				if (block[i].Position.x - collitionRadius < g_Position.x + ballRadius &&
					g_Position.x - ballRadius < block[i].Position.x + collitionRadius)
				{
					if (block[i].Position.x < g_Position.x)
					{
						//右側から衝突
						g_Position.x = block[i].Position.x + collitionRadius + ballRadius;
					}
					else
					{
						//左側から衝突
						g_Position.x = block[i].Position.x - collitionRadius - ballRadius;
					}
					//g_Velocity.x -= g_Velocity.x * 2.0f * dt;
					g_Velocity.x *= -0.5;//反発係数
				}
			}
			else if (block[i].Position.x - collitionRadius < g_Position.x &&
				g_Position.x < block[i].Position.x + collitionRadius)
			{
				//Z方向
				if (block[i].Position.z - collitionRadius < g_Position.z + ballRadius &&
					g_Position.z - ballRadius < block[i].Position.z + collitionRadius)
				{
					if (block[i].Position.z < g_Position.z)
					{
						//手前から衝突
						g_Position.z = block[i].Position.z + collitionRadius + ballRadius;
					}
					else
					{
						//奥から衝突
						g_Position.z = block[i].Position.z - collitionRadius - ballRadius;
					}
					g_Velocity.z *= -0.5f;//反発係数
				}
			}
		}
		else
		{
			//縦方向
			if (block[i].Position.z - collitionRadius < g_Position.z &&
				g_Position.z < block[i].Position.z + collitionRadius)
			{
				if (block[i].Position.x - collitionRadius < g_Position.x &&
					g_Position.x < block[i].Position.x + collitionRadius)
				{
					if (block[i].Position.y - collitionRadius < g_Position.y + ballRadius &&
						g_Position.y - ballRadius < block[i].Position.y + collitionRadius)
					{
						if (block[i].Position.y < g_Position.y)
						{
							g_Position.y = block[i].Position.y + collitionRadius + ballRadius;
						}
						else
						{
							g_Position.y = block[i].Position.y - collitionRadius - ballRadius;
						}
						if (Keyboard_IsKeyDown(KK_A))
						{
							g_Velocity.x -= cameraforward.z * 10.0f * dt;
							g_Velocity.z += cameraforward.x * 10.0f * dt;
						}
						if (Keyboard_IsKeyDown(KK_D))
						{
							g_Velocity.x += cameraforward.z * 10.0f * dt;
							g_Velocity.z -= cameraforward.x * 10.0f * dt;
						}
						if (Keyboard_IsKeyDown(KK_W))
						{
							g_Velocity.x += cameraforward.x * 10.0f * dt;
							g_Velocity.z += cameraforward.z * 10.0f * dt;
						}
						if (Keyboard_IsKeyDown(KK_S))
						{
							g_Velocity.x -= cameraforward.x * 10.0f * dt;
							g_Velocity.z -= cameraforward.z * 10.0f * dt;
						}
						////ベクトルの長さ
						//float forcelength = sqrtf(force.x * force.x
						//	+ force.y * force.y
						//	+ force.z * force.z);

						//if (forcelength > 1.0f)
						//{
						//	//正規化
						//	force.x /= forcelength;
						//	force.z /= forcelength;
						//}
						
						//g_Velocity.x -= g_Velocity.x * 3.0f * dt;
						//g_Velocity.z -= g_Velocity.z * 3.0f * dt;

						//接地摩擦
						//g_Velocity.x -= g_Velocity.x * 3.0f * dt;
						//g_Velocity.z -= g_Velocity.z * 3.0f * dt;

						g_Velocity.y *= -0.7f;//反発係数
						if (Keyboard_IsKeyTrigger(KK_SPACE))
						{
							//ジャンプ
							g_Velocity.y += 7.0f;
						}
					}
				}
			}
		}
	}
	for (int x = 0; x < ItemCount; x++)
	{
		if (item[x].Position.y - collitionRadius < g_Position.y &&
			g_Position.y < item[x].Position.y + collitionRadius)
		{
			//横方向
			if (item[x].Position.z - collitionRadius < g_Position.z &&
				g_Position.z < item[x].Position.z + collitionRadius)
			{
				if (item[x].Position.x - collitionRadius < g_Position.x + ballRadius &&
					g_Position.x - ballRadius < item[x].Position.x + collitionRadius)
				{
					if (item[x].Position.x < g_Position.x)
					{
						//右側から衝突
						g_Position.x = item[x].Position.x + collitionRadius + ballRadius;
					}
					else
					{
						//左側から衝突
						g_Position.x = item[x].Position.x - collitionRadius - ballRadius;
					}
					//g_Velocity.x -= g_Velocity.x * 2.0f * dt;
					g_Velocity.x *= -0.5;//反発係数
				}
			}
			else if (item[x].Position.x - collitionRadius < g_Position.x &&
				g_Position.x < item[x].Position.x + collitionRadius)
			{
				//Z方向
				if (item[x].Position.z - collitionRadius < g_Position.z + ballRadius &&
					g_Position.z - ballRadius < item[x].Position.z + collitionRadius)
				{
					if (item[x].Position.z < g_Position.z)
					{
						//手前から衝突
						g_Position.z = item[x].Position.z + collitionRadius + ballRadius;
					}
					else
					{
						//奥から衝突
						g_Position.z = item[x].Position.z - collitionRadius - ballRadius;
					}
					g_Velocity.z *= -0.5f;//反発係数
				}
			}
		}
		else
		{
			//縦方向
			if (item[x].Position.z - collitionRadius < g_Position.z &&
				g_Position.z < item[x].Position.z + collitionRadius)
			{
				if (item[x].Position.x - collitionRadius < g_Position.x &&
					g_Position.x < item[x].Position.x + collitionRadius)
				{
					if (item[x].Position.y - collitionRadius < g_Position.y + ballRadius &&
						g_Position.y - ballRadius < item[x].Position.y + collitionRadius)
					{
						if (item[x].Position.y < g_Position.y)
						{
							g_Position.y = item[x].Position.y + collitionRadius + ballRadius;
						}
						else
						{
							g_Position.y = item[x].Position.y - collitionRadius - ballRadius;
						}
						if (Keyboard_IsKeyDown(KK_A))
						{
							//g_Velocity.x -= 10.0f * dt;
							g_Velocity.x -= cameraforward.z * 5.0f * dt;
							g_Velocity.z += cameraforward.x * 5.0f * dt;
						}
						if (Keyboard_IsKeyDown(KK_D))
						{
							//g_Velocity.x += 10.0f * dt;
							g_Velocity.x += cameraforward.z * 5.0f * dt;
							g_Velocity.z -= cameraforward.x * 5.0f * dt;
						}
						if (Keyboard_IsKeyDown(KK_W))
						{
							//g_Velocity.z += 10.0f * dt;
							g_Velocity.x += cameraforward.x * 5.0f * dt;
							g_Velocity.z += cameraforward.z * 5.0f * dt;
						}
						if (Keyboard_IsKeyDown(KK_S))
						{
							//g_Velocity.z -= 10.0f * dt;
							g_Velocity.x -= cameraforward.x * 5.0f * dt;
							g_Velocity.z -= cameraforward.z * 5.0f * dt;
						}
						////ベクトルの長さ
						//float forcelength = sqrtf(force.x * force.x
						//	+ force.y * force.y
						//	+ force.z * force.z);

						//if (forcelength > 1.0f)
						//{
						//	//正規化
						//	force.x /= forcelength;
						//	force.z /= forcelength;
						//}

						//接地摩擦
						//g_Velocity.z -= g_Velocity.z * 2.0f * dt;
						//g_Velocity.x -= g_Velocity.x * 2.0f * dt;

						g_Velocity.y *= -0.3f;//反発係数
						if (Keyboard_IsKeyTrigger(KK_SPACE))
						{
							//ジャンプ
							g_Velocity.y += 7.0f;
						}
					}
				}
			}
		}
	}
}