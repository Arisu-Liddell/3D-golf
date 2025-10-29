#include "directx.h"
#include "game.h"
#include "score.h"
#include "keyboard.h"
#include "sprite.h"
#include "pause.h"
#include "cube.h"
#include "camera.h"



static int g_BGM;
static bool g_Pause;

void GameInitialize(void)
{
	CameraInitialize();
	PauseInitialize();
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
	CubeUpdate();
	ScoreUpdate();

}

void GameDraw(void)
{
	CameraDraw();
	//if (g_Pause)
	//{
	//	//ï`âÊèàóù
	//	PauseDraw();
	//	ScoreDraw();
	//}
	//if (!g_Pause)
	//{
	//	//ï`âÊèàóù

	//}
	CubeDraw();
	//ScoreDraw();
}


void GameFinalize(void)
{
	CameraDraw();
	CubeFinalize();
	ScoreFinalize();
}