#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "ball.h"
#include "field.h"

static MODEL* g_Model = NULL;

static XMFLOAT3 g_Position;//位置
static XMFLOAT3 g_Rotation;//回転
static XMFLOAT3 g_Velocity;//速度

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
	float dt = 1.0f / 60.0f;
	g_Rotation.y += 0.01f;
	if (Keyboard_IsKeyDown(KK_A))
	{

		g_Velocity.x -= 5.0f * dt;

	}
	if (Keyboard_IsKeyDown(KK_D))
	{


		g_Velocity.x += 5.0f * dt;

	}
	if (Keyboard_IsKeyDown(KK_W))
	{

		g_Velocity.z += 5.0f * dt;

	}
	if (Keyboard_IsKeyDown(KK_S))
	{

		g_Velocity.z -= 5.0f * dt;

	}



	//重力
	g_Velocity.y -= 9.8f * dt;
	//摩擦
	g_Velocity.x -= g_Velocity.x * 2.0f * dt;
	g_Velocity.y -= g_Velocity.y * 0.1f * dt;
	g_Velocity.z -= g_Velocity.z * 2.0f * dt;
	//移動
	g_Position.x += g_Velocity.x * dt;
	g_Position.y += g_Velocity.y * dt;
	g_Position.z += g_Velocity.z * dt;

	//当たり判定
	BallHitCheck();
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
	BLOCK * block = GetFieldBlock();
	BLOCK * item = GetFieldItem();

	float blockRadius = 0.5f;
	float ballRadius = 0.2f;
	float e = 0.5f;

	for (int i= 0; i < GridCount; i++)
	{
		if (block[i].Position.y - blockRadius < g_Position.y &&
			g_Position.y < block[i].Position.y + blockRadius)
		{
			//横方向
			if(block[i].Position.z - blockRadius < g_Position.z &&
				g_Position.z < block[i].Position.z + blockRadius)
			{
				if (block[i].Position.x - blockRadius < g_Position.x + ballRadius &&
					g_Position.x - ballRadius < block[i].Position.x + blockRadius)
				{
					if(block[i].Position.x < g_Position.x)
					{
						//右側から衝突
						g_Position.x = block[i].Position.x + blockRadius + ballRadius;
					}
					else
					{
						//左側から衝突
						g_Position.x = block[i].Position.x - blockRadius - ballRadius;
					}
					g_Velocity.x *= -0.5;//反発係数
				}
			}
			else if(block[i].Position.x - blockRadius < g_Position.x &&
				g_Position.x < block[i].Position.x + blockRadius)
			{
				//Z方向
				if (block[i].Position.z - blockRadius < g_Position.z + ballRadius &&
					g_Position.z - ballRadius < block[i].Position.z + blockRadius)
				{
					if (block[i].Position.z < g_Position.z)
					{
						//手前から衝突
						g_Position.z = block[i].Position.z + blockRadius + ballRadius;
					}
					else
					{
						//奥から衝突
						g_Position.z = block[i].Position.z - blockRadius - ballRadius;
					}
					g_Velocity.z *= -0.5f;//反発係数
				}
			}
		}
		else
		{
			//縦方向
			if (block[i].Position.z - blockRadius < g_Position.z &&
				g_Position.z < block[i].Position.z + blockRadius)
			{
				if (block[i].Position.x - blockRadius < g_Position.x &&
					g_Position.x < block[i].Position.x + blockRadius)
				{
					if (block[i].Position.y - blockRadius < g_Position.y + ballRadius &&
						g_Position.y - ballRadius < block[i].Position.y + blockRadius)
					{
						if (block[i].Position.y < g_Position.y)
						{
							g_Position.y = block[i].Position.y + blockRadius + ballRadius;
						}
						else
						{
							g_Position.y = block[i].Position.y - blockRadius - ballRadius;
						}
						g_Velocity.y *= -0.5f;//反発係数
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
	for(int x = 0; x < ItemCount; x++)
	{
		if (item[x].Position.y - blockRadius < g_Position.y &&
			g_Position.y < item[x].Position.y + blockRadius)
		{
			//横方向
			if (item[x].Position.z - blockRadius < g_Position.z &&
				g_Position.z < item[x].Position.z + blockRadius)
			{
				if (item[x].Position.x - blockRadius < g_Position.x + ballRadius &&
					g_Position.x - ballRadius < item[x].Position.x + blockRadius)
				{
					if (item[x].Position.x < g_Position.x)
					{
						//右側から衝突
						g_Position.x = item[x].Position.x + blockRadius + ballRadius;
					}
					else
					{
						//左側から衝突
						g_Position.x = item[x].Position.x - blockRadius - ballRadius;
					}
					g_Velocity.x *= -0.5f;//反発係数
				}
			}
			else if (item[x].Position.x - blockRadius < g_Position.x &&
				g_Position.x < item[x].Position.x + blockRadius)
			{
				//Z方向
				if (item[x].Position.z - blockRadius < g_Position.z + ballRadius &&
					g_Position.z - ballRadius < item[x].Position.z + blockRadius)
				{
					if (item[x].Position.z < g_Position.z)
					{
						//手前から衝突
						g_Position.z = item[x].Position.z + blockRadius + ballRadius;
					}
					else
					{
						//奥から衝突
						g_Position.z = item[x].Position.z - blockRadius - ballRadius;
					}
					g_Velocity.z *= -0.5f;//反発係数
				}
			}
		}
		else
		{
			if (item[x].Position.z - blockRadius < g_Position.z &&
				g_Position.z < item[x].Position.z + blockRadius)
			{
				if (item[x].Position.x - blockRadius < g_Position.x &&
					g_Position.x < item[x].Position.x + blockRadius)
				{
					if (item[x].Position.y - blockRadius < g_Position.y + ballRadius &&
						g_Position.y - ballRadius < item[x].Position.y + blockRadius)
					{
						if (item[x].Position.y < g_Position.y)
						{
							g_Position.y = item[x].Position.y + blockRadius + ballRadius;
						}
						else
						{
							g_Position.y = item[x].Position.y - blockRadius - ballRadius;
						}
						g_Velocity.y *= -0.5f;
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