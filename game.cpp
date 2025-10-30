#include "directx.h"
#include "game.h"
#include "score.h"
#include "keyboard.h"
#include "sprite.h"
#include "pause.h"
#include "cube.h"
#include "camera.h"
#include "ball.h"



static int g_BGM;
static bool g_Pause;

void GameInitialize(void)
{
	CameraInitialize();
	PauseInitialize();
	BallInitialize();
	CubeInitialize();

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
	PauseDraw();
	BallDraw();
	CubeUpdate();
	ScoreUpdate();

}

void GameDraw(void)
{
	SetDepthEnable(true);

	CameraDraw();
	CubeDraw();
	BallDraw();

	SetDepthEnable(false);

	ScoreDraw();
}


void GameFinalize(void)
{
	CameraDraw();
	CubeFinalize();
	BallFinalize();

	ScoreFinalize();
}