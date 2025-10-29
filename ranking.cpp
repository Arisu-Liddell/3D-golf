#include "directx.h"
#include "ranking.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"


static int g_Texture, g_Ranking[4] = { 10,5,1,0 };


void SetRanking(int Ranking)
{
	g_Ranking[3] = Ranking; //スコアを加算

	for (int i = 3; i > 0; i--)
	{
		if (g_Ranking[i] > g_Ranking[i - 1])
		{
			int ranking = g_Ranking[i];
			g_Ranking[i] = g_Ranking[i - 1];
			g_Ranking[i - 1] = ranking;
		}
	}
}

void RankingInitialize(void)
{
	g_Texture = TextureLoad(L"asset\\texture\\number.png"); //テクスチャの読み込み

//	g_Ranking = 0;
}

void RankingFinalize(void)
{

}

void RankingUpdate(void)
{

}

//ポリゴン描画
void RankingDraw(void)
{
	//テクスチャの設定
	ID3D11ShaderResourceView* texture = GetTexture(g_Texture); //テクスチャの取得
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定
	for (int j = 0; j < 3; j++)
	{
		int value = g_Ranking[j];
		for (int i = 0; i < 5; i++)
		{
			int num = value % 10; //一の位を取得
			value /= 10; //次の桁へ

			float tx = (num % 5) / 5.0f; //テクスチャのX座標
			float tw = 1.0f / 5.0f; //テクスチャのY座標
			float ty = (num / 5) / 5.0f; //テクスチャの幅
			float th = 1.0f / 5.0f; //テクスチャの高さ


			SpriteDraw(
				300.0f - i * 50.0f, 100.0f + j * 100.0f, 100.0f, 100.0f,
				tx, ty,
				tw, th, 0.0f);
		}
	}
}

