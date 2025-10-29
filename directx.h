#ifndef DIRECTX_H//include guard
#define DIRECTX_H

#define NOMINMAX
#include <Windows.h>
#include <d3d11.h>//DirectX11のヘッダファイル
#include <DirectXMath.h>//ベクトルやマトリクスを扱うためのヘッダファイル
using namespace DirectX;//DirectXMathを使うための名前空間指定

#define SAFE_RELEASE(o) if(o){ (o)->Release(); o = NULL; }//オブジェクトの解放マクロ

void DirectXInitialize(HWND hWnd);//立ち上げ処理
void DirectXFinalize(void);//終了処理

ID3D11Device* DirectXGetDevice(void);//デバイス取得
ID3D11DeviceContext* DirectXGetDeviceContext(void);//デバイスコンテキスト取得

void Clear(void);//画面クリア
void Present(void);//画面更新

//void SetBlend;
void SetDepthEnable(bool DepthEnable);//Zバッファスイッチ

#define SCREEN_WIDTH (1280)//画面幅
#define SCREEN_HEIGHT (720)//画面高さ

#define BLOCK_WIDTH  (50)
#define BLOCK_HEIGHT (50)//1つのピースの幅と高さ

#define BLOCK_COLS (6) //ブロックの列数
#define BLOCK_ROWS (13) //ブロックの行数

#endif
