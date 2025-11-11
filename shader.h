/*==============================================================================

   シェーダー [shader.h]
														 Author : Youhei Sato
														 Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SHADER_H
#define	SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>

struct LIGHT
{
	BOOL LightEnable;    // ライトの有効・無効 大文字のBOOL（type def intと同じバイト数)でないとダメ、大文字でないとバイト数が合わない
	float Dummy0[3];//きっも

	XMFLOAT3	LightDirection; // ライトの向き
	float Dummy1;//きっも
};

bool Shader_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);//シェーダー初期化
void Shader_Finalize();//シェーダー終了

void Shader_SetMatrix(const DirectX::XMMATRIX& matrix);//行列設定
void Shader_SetLight(const LIGHT& light);//ライト設定
void Shader_Begin();//シェーダー開始



#endif // SHADER_H
