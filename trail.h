#pragma once
#define g_textureMAX 256 //テクスチャ最大数

void TrailInitialize(void);//ポリゴン初期化
void TrailFinalize(void);//ポリゴン終了
void TrailUpdate(void);//ポリゴン更新
void TrailDraw(void);//ポリゴン描画

void CreateTrail(XMFLOAT3 Position);//エフェクト生成
void SetTrailPosition(XMFLOAT3 Position);

