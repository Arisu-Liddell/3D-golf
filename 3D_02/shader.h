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

bool Shader_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);//シェーダー初期化
void Shader_Finalize();//シェーダー終了

void Shader_SetMatrix(const DirectX::XMMATRIX& matrix);//行列設定
void Shader_Begin();//シェーダー開始

#endif // SHADER_H
