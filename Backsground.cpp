#include "directx.h"
#include "Background.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "keyboard.h"


static int g_Texture;
static int g_Texturebutton;
static float g_ButtonTime;

void BackgroundInitialize(void)
{
	//テクスチャの読み込み
	g_Texture = TextureLoad(L"asset\\texture\\bg_01.png"); //テクスチャの読み込み
}

void BackgroundFinalize(void)
{
}

void BackgroundUpdate(void)
{
}

//ポリゴン描画
void BackgroundDraw(void)
{
	//テクスチャの設定
	ID3D11ShaderResourceView* texture = GetTexture(g_Texture); //テクスチャの取得
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定

	SpriteDraw(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f);
}

