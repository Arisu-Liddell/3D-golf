#pragma once
#define g_textureMAX 256 //テクスチャ最大数

void RankingInitialize(void);//ポリゴン初期化
void RankingFinalize(void);//ポリゴン終了
void RankingUpdate(void);//ポリゴン更新
void RankingDraw(void);//ポリゴン描画

void SetRanking(int Ranking);//スコア設定

