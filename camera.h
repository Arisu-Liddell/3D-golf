#pragma once
void CameraInitialize(void);//ポリゴン初期化
void CameraFinalize(void);//ポリゴン終了
void CameraUpdate(void);//ポリゴン更新
void CameraDraw(void);//ポリゴン描画
void CameraShake(float Tremor);//カメラシェイク

XMMATRIX GetCameraViewMatrix(void);
XMMATRIX GetCameraProjectionMatrix(void);
XMFLOAT3 GetCameraForward(void);
XMFLOAT3 GetCameraPosition(void);

