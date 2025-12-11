#include "directx.h"
#include "title.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "keyboard.h"
#include <iostream>

enum TITLE_STATE
{
	TITLE_STATE_STAY,
	TITLE_STATE_START
};
static TITLE_STATE g_State;

static int g_Texture;
static int g_Texturebutton;
static float g_ButtonTime;
static bool success = false;


void TitleInitialize(void)
{
	//テクスチャの読み込み
	g_Texture = TextureLoad(L"asset\\texture\\title.png"); //テクスチャの読み込み
	g_Texturebutton = TextureLoad(L"asset\\texture\\button.png"); //テクスチャの読み込み
	g_ButtonTime = 0.0f;
	g_State = TITLE_STATE_STAY;
	success = false;
}

void TitleFinalize(void)
{
}

void TitleUpdate(void)
{
	g_ButtonTime = g_ButtonTime + 0.01f;
	if (g_ButtonTime > 6.2831853f) g_ButtonTime -= 6.2831853f;
	//ステートマシン
	switch (g_State)
	{
	case TITLE_STATE_STAY:
		if (Keyboard_IsKeyTrigger(KK_ENTER))//エンターキーが押されたら
		{
			g_State = TITLE_STATE_START;
		}
		break;
	case TITLE_STATE_START:
			if (success == false)
			{
				Transition(SCENE_GAME);
			}
			success = true;
		break;
	default:
		break;
	}
}

//ポリゴン描画
void TitleDraw(void) 
{
	const float Button_Center_Y = SCREEN_HEIGHT * 0.73f;
	const float Button_Aplitude = 25.0f;

	//テクスチャの設定
	ID3D11ShaderResourceView* texture = GetTexture(g_Texture); //テクスチャの取得
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定

	SpriteDraw(SCREEN_WIDTH /2,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT,
		0.0f,0.0f,1.0f,1.0f);


	float buttonPosY = Button_Center_Y + sinf(g_ButtonTime) * Button_Aplitude;

	texture = GetTexture(g_Texturebutton); //テクスチャの取得
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定

	SpriteDraw(SCREEN_WIDTH/2, buttonPosY, 400.0f,300.0f,
				0.0f, 0.0f, 1.0f, 1.0f);
}



void InitDirectX()
{
	std::cout << "[DX] placeholder" << std::endl;
}