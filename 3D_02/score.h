#pragma once
#define g_textureMAX 256 //テクスチャ最大数

void ScoreInitialize(void);//ポリゴン初期化
void ScoreFinalize(void);//ポリゴン終了
void ScoreUpdate(void);//ポリゴン更新
void ScoreDraw(void);//ポリゴン描画
void ScoreAdd(int Score);//スコア加算
int GetScore(void);//スコア取得


