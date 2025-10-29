#include "directx.h"
#include "score.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"

static int g_Texture, g_Score;

int GetScore(void)
{
	return g_Score;
}
void ScoreAdd(int Score)
{
	g_Score += Score; //スコアを加算
}

void ScoreInitialize(void)
{
	g_Texture = TextureLoad(L"asset\\texture\\number.png"); //テクスチャの読み込み

	g_Score = 0;
}

void ScoreFinalize(void)
{

}

void ScoreUpdate(void)
{

}

//ポリゴン描画
void ScoreDraw(void)
{
	//テクスチャの設定
	ID3D11ShaderResourceView* texture = GetTexture(g_Texture); //テクスチャの取得
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定
	
	int value = g_Score;

	for (int i = 0; i < 5; i++)
	{
		int num = value % 10; //一の位を取得
		value /= 10; //次の桁へ

		float tx = (num % 5) / 5.0f; //テクスチャのX座標
		float tw = 1.0f / 5.0f; //テクスチャのY座標
		float ty = (num / 5) / 5.0f; //テクスチャの幅
		float th = 1.0f / 5.0f; //テクスチャの高さ


		SpriteDraw(
			300.0f - i * 50.0f, 100.0f, 100.0f, 100.0f,
			tx, ty,
			tw, th, 0.0f);
	}
}

