#include "directx.h"
#include "result.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "keyboard.h"
#include "score.h"
#include "main.h" 
#include "ranking.h"

// リザルト用テクスチャ
static int   g_TexResultBg = -1;
static int   g_TexResultBtn = -1;
static float g_ButtonTime = 0.0f;

void ResultInitialize(void)
{
    RankingInitialize();
    // 背景とボタン
    g_TexResultBg = TextureLoad(L"asset\\texture\\result.png");
    g_TexResultBtn = TextureLoad(L"asset\\texture\\execute.png");

    g_ButtonTime = 0.0f;
}

void ResultFinalize(void)
{
 RankingFinalize();
}

void ResultUpdate(void)
{
    //// ENTER でタイトルへ戻る
    //if (Keyboard_IsKeyTrigger(KK_ENTER)) {
    //    SetScene(SCENE_TITLE);
    //    return;
    //}

    RankingUpdate();

    // ボタンのふわふわ上下
    g_ButtonTime += 0.03f;              // 速度は好みで調整可
    if (g_ButtonTime > 6.2831853f) {    // 2πでループ
        g_ButtonTime -= 6.2831853f;
    }

    if (Keyboard_IsKeyTrigger(KK_ENTER))//エンターキーが押されたら
    {
        Transition(SCENE_TITLE);//タイトルからゲームへ
    }
}

void ResultDraw(void)
{
    // 背景
    {
        ID3D11ShaderResourceView* tex = GetTexture(g_TexResultBg);
        DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &tex);

        SpriteDraw(
            SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,
            (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT,
            0.0f, 0.0f, 1.0f, 1.0f
        );

    }
    // スコア表示（ScoreDraw は内部で number.png を使って描画）
  //  ScoreDraw();
      RankingDraw();
    // 「PRESS ENTER」想定のボタン（上下にゆっくり浮遊）
    {
        const float centerY = SCREEN_HEIGHT * 0.85f;
        const float amplitude = 25.0f;
        const float buttonY = centerY + sinf(g_ButtonTime) * amplitude;

        ID3D11ShaderResourceView* tex = GetTexture(g_TexResultBtn);
        DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &tex);

        SpriteDraw(
            SCREEN_WIDTH * 0.5f, buttonY,
            400.0f, 300.0f,
            0.0f, 0.0f, 1.0f, 1.0f
        );
    }
}
