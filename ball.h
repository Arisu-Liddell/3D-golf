#pragma once

void BallInitialize(void);//ポリゴン初期化
void BallFinalize(void);//ポリゴン終了
void BallUpdate(void);//ポリゴン更新
void BallDraw(void);//ポリゴン描画
void BallHitCheck(void);//ポリゴン当たり判定

XMFLOAT3 GetBallPosition(void);