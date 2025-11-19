#include "directx.h"
#include "game.h"
#include "score.h"
#include "keyboard.h"
#include "sprite.h"
#include "pause.h"
#include "cube.h"
#include "camera.h"
//#include "ball.h"
#include "shader.h"
#include "field.h"	
#include "Background.h"



static int g_BGM;
static bool g_Pause;

void GameInitialize(void)
{
	CameraInitialize();
	PauseInitialize();
	//BallInitialize();
	//CubeInitialize();
	InitPositions();
	FieldInitialize();
	BackgroundInitialize();
	ScoreInitialize();
}


void GameUpdate(void)
{
	CameraUpdate();
	//if (Keyboard_IsKeyTrigger(KK_ESCAPE))
	//{
	//	g_Pause = !g_Pause;
	//}
	//if (!g_Pause)
	//{
	//}
	PauseUpdate();
	//BallUpdate();
	//CubeUpdate();
	FieldUpdate();
	BackgroundUpdate();
	ScoreUpdate();

}

void GameDraw(void)
{

	LIGHT light;
	SetDepthEnable(true);
	light.LightEnable = FALSE;
	light.LightEnable = TRUE;
	//light.LightDirection = { 0.0f,-1.0f,0.0f };
	XMVECTOR direction = { 0.3f,-1.0f, 0.5f };
	direction = XMVector3Normalize(direction);//　正規化する関数
	XMStoreFloat3(&light.LightDirection, direction);

	Shader_SetLight(light);

	
	CameraDraw();
	//CubeDraw();
	FieldDraw();
	//BallDraw();

	SetDepthEnable(false);

	light.LightEnable = FALSE;
	ScoreDraw();;
	BackgroundDraw();
}


void GameFinalize(void)
{
	BackgroundFinalize();
	CameraFinalize();
	//CubeFinalize();
	FieldFinalize();
	//BallFinalize();

	ScoreFinalize();
}
