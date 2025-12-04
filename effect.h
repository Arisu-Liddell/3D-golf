#pragma once
#define g_textureMAX 256 //テクスチャ最大数

void EffectInitialize(void);//ポリゴン初期化
void EffectFinalize(void);//ポリゴン終了
void EffectUpdate(void);//ポリゴン更新
void EffectDraw(void);//ポリゴン描画

void CreateEffect(XMFLOAT3 Position);//エフェクト生成



#pragma once
