#include "directx.h"
#include "game.h"
#include "score.h"
#include "keyboard.h"
#include "sprite.h"
#include "pause.h"
#include "cube.h"
#include "camera.h"
#include "ball.h"
#include "Objball.h"
#include "shader.h"
#include "field.h"	
#include "goal.h"
#include "Background.h"
#include "effect.h"
#include "trail.h"
#include "ObjTrail.h"
#include "BillBordTest.h"
#include "Shadow.h"
#include "Map.h"
#include "Skydome.h"




static int g_BGM;
static bool g_Pause;

void GameInitialize(void)
{
	CameraInitialize();
	PauseInitialize();
	BallInitialize();
//	ObjballInitialize();
	//CubeInitialize();
	FieldInitialize();
	GoalInitialize();
	BackgroundInitialize();
	ScoreInitialize();
	EffectInitialize();
	TrailInitialize();
//	ObjTrailInitialize();
	BillBordInitialize();
	ShadowInitialize();
	MapInitialize();
	SkyDomeInitialize();

}


void GameUpdate(void)
{
	CameraUpdate();
	PauseUpdate();
	SkyDomeUpdate();
	BallUpdate();
//	ObjballUpdate();
	//CubeUpdate();
	FieldUpdate();
	GoalUpdate();
	BackgroundUpdate();
	ScoreUpdate();
	EffectUpdate();
	TrailUpdate();
//	ObjTrailUpdate();
	BillBordUpdate();
	ShadowUpdate();
}

void GameDraw(void)
{
    LIGHT light;

    // --- ここで必ず「通常描画の状態」に戻す ---
    SetRenderTarget();        // RT + DSV を必ず設定（MapDraw等で変えても戻す）
    SetDepthEnable(true);     // 深度ON（※DepthDisable側のWriteMask=ZERO前提）
    // ※ Clear() は GameDraw の外（1フレーム先頭）で呼んでいる想定。
    //   もし呼んでいないなら、ここで Clear() を呼ぶ（ただし1回だけにする）。

    // ----------------------------
    // ライト（方向ライト）
    // ----------------------------
    light.LightEnable = true;

    XMVECTOR direction = { 0.3f, -1.0f, 0.5f };
    direction = XMVector3Normalize(direction);
    XMStoreFloat3(&light.LightDirection, direction);

    Shader_SetLight(light);

    // カメラ関連（必要ならここで行列更新）
    CameraDraw();

    // ----------------------------
    // スカイドーム：Depth OFF 推奨
    // （背景なのでZを書かない/テストしない）
    // ----------------------------
    SetDepthEnable(false);
    light.LightEnable = false;
    Shader_SetLight(light);
    SkyDomeDraw();

    // ----------------------------
    // 3Dオブジェクト：Depth ON
    // ----------------------------
    SetDepthEnable(true);
    light.LightEnable = true;
    Shader_SetLight(light);

    FieldDraw();
    BallDraw();
    GoalDraw();

    // ----------------------------
    // 影・トレイル・ビルボード等（あなたの仕様に合わせる）
    // 影はDepth ONのままが安全なことが多い（地面に落とすなら）
    // ----------------------------
    light.LightEnable = FALSE;
    Shader_SetLight(light);

    SetDepthEnable(true);
    ShadowDraw();

    // トレイルは半透明ならDepth OFF or DepthWrite OFFが欲しくなるが、
    // いまはSetDepthEnableしか無いので、まずは OFF で描いて破綻を避ける
    SetDepthEnable(false);
    TrailDraw();
    BillBordDraw();
    EffectDraw();

    // ----------------------------
    // 2D/UI：Depth OFF
    // ----------------------------
    SetDepthEnable(false);
    ScoreDraw();

    // ----------------------------
    // ミニマップ：基本はDepth ON
    // ただし MapDraw が viewport/RT を変えるなら内部で戻すか、
    // ここで最後に必ず戻す
    // ----------------------------
    SetDepthEnable(true);
    MapDraw();

    // MapDrawで viewport/RT を変える場合に備えて、最後に戻す
    SetRenderTarget();
    SetDepthEnable(false);
}



void GameFinalize(void)
{
	SkyDomeFinalize();
	MapFinalize();
	EffectFinalize();
	TrailFinalize();
//	ObjTrailFinalize();
	ShadowFinalize();
	BillBordFinalize();
//	BackgroundFinalize();
	CameraFinalize();
	GoalFinalize();
	//CubeFinalize();
	FieldFinalize();
//	ObjballFinalize();
	BallFinalize();
	ScoreFinalize();
}
