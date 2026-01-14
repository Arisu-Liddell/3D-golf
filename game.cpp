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
#include "effect.h"
#include "trail.h"
#include "BillBordTest.h"
#include "Shadow.h"



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
	EffectInitialize();
	TrailInitialize();
	BillBordInitialize();
	ShadowInitialize();


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
	EffectUpdate();
	TrailUpdate();
	BillBordUpdate();
	ShadowUpdate();
}

void GameDraw(void)
{
	LIGHT light;
	XMVECTOR direction = { 0.3f,-1.0f, 0.5f };
	direction = XMVector3Normalize(direction);//　正規化する関数
	XMStoreFloat3(&light.LightDirection, direction);

	BackgroundDraw();

	//ライトオン
	light.LightEnable = TRUE;
	Shader_SetLight(light);//ライト設定

	SetDepthEnable(true);//Zバッファ有効化
	CameraDraw();
	//CubeDraw();
	FieldDraw();
	BallDraw();
	GoalDraw();

	//ライトオフ
	light.LightEnable = FALSE;
	Shader_SetLight(light);//ライト設定

	ShadowDraw();

	TrailDraw();

	BillBordDraw();
	EffectDraw();//他のオブジェクトの後に

	SetDepthEnable(false);//
	ScoreDraw();;
}


void GameFinalize(void)
{
	EffectFinalize();
	TrailFinalize();
	ShadowFinalize();
	BillBordFinalize();
	BackgroundFinalize();
	CameraFinalize();
	GoalFinalize();
	//CubeFinalize();
	FieldFinalize();
	BallFinalize();

	ScoreFinalize();
}
