#pragma once
#define g_TrailtextureMAX 256 //テクスチャ最大数

void ObjTrailInitialize(void);//ポリゴン初期化
void ObjTrailFinalize(void);//ポリゴン終了
void ObjTrailUpdate(void);//ポリゴン更新
void ObjTrailDraw(void);//ポリゴン描画

void CreateObjTrail(XMFLOAT3 Position);//エフェクト生成
void SetObjTrailPosition(XMFLOAT3 Position);
void ResetObjTrailPosition(XMFLOAT3 Position);
