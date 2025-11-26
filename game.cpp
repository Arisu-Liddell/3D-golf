#include "directx.h"
#include "game.h"
#include "score.h"
#include "keyboard.h"
#include "sprite.h"
#include "pause.h"
#include "cube.h"
#include "camera.h"
#include "ball.h"
#include "shader.h"
#include "field.h"	
#include "goal.h"
#include "Background.h"



static int g_BGM;
static bool g_Pause;

void GameInitialize(void)
{
	CameraInitialize();
	PauseInitialize();
	BallInitialize();
	//CubeInitialize();
	FieldInitialize();
	GoalInitialize();
	BackgroundInitialize();
	ScoreInitialize();
}


void GameUpdate(void)
{
	CameraUpdate();
	PauseUpdate();
	BallUpdate();
	//CubeUpdate();
	FieldUpdate();
	GoalUpdate();
	BackgroundUpdate();
	ScoreUpdate();

}

void GameDraw(void)
{
	//BackgroundDraw();

	LIGHT light;
	//light.LightDirection = { 0.0f,-1.0f,0.0f };
	XMVECTOR direction = { 0.3f,-1.0f, 0.5f };
	direction = XMVector3Normalize(direction);//　正規化する関数
	XMStoreFloat3(&light.LightDirection, direction);

	Shader_SetLight(light);//ライト設定
	SetDepthEnable(true);//Zバッファ有効化
	light.LightEnable = TRUE;
	
	CameraDraw();
	//CubeDraw();
	FieldDraw();
	BallDraw();
	GoalDraw();

	light.LightEnable = FALSE;
	SetDepthEnable(false);//

	ScoreDraw();;
}


void GameFinalize(void)
{
	BackgroundFinalize();
	CameraFinalize();
	GoalFinalize();
	//CubeFinalize();
	FieldFinalize();
	BallFinalize();

	ScoreFinalize();
}
