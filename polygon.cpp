#include "directx.h"
#include "polygon.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "KeyBoard.h"

static int g_Texture[g_textureMAX], g_Count, g_frame;
static float g_PolygonX, g_PolygonY, g_PolygonRot, g_width,g_height;

void PolygonInitialize(void)
{
	//g_Texture[0] = TextureLoad(L"asset\\texture\\test.png"); //テクスチャの読み込み

	g_Count = 0;
	g_frame = 0;
	g_PolygonX = 0.0f;
	g_PolygonY = 0.0f;
	g_PolygonRot = 0.0f;
	g_width = 0.5f;
	g_height = 0.5;
}

void PolygonFinalize(void)
{
}

void PolygonUpdate(void)
{
	 g_Count++; //フレーム数を取得

	if (g_Count > 10) //60フレームごとにテクスチャの座標を更新1
	{
		g_frame++; //テクスチャの上の座標をリセット
		g_frame = g_frame % 10; //剰余算でテクスチャの上の座標をリセット
		g_Count = 0; 
	}


}

//ポリゴン描画
void PolygonDraw(void)
{
	//テクスチャの設定
	ID3D11ShaderResourceView* texture = GetTexture(g_Texture[0]); //テクスチャの取得
	DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定

	float tx = (g_frame % 5) / 5.0f; //テクスチャのX座標
	float tw = 1.0f / 5.0f; //テクスチャのY座標
	float ty = (g_frame / 5) * 1.0f / 2.0f; //テクスチャのY座標
	float th = 1.0f / 2.0f; //テクスチャの高さ


	SpriteDraw(
		g_PolygonX +100.0f, g_PolygonY+100.0f,
		400.0f, 600.0f,
		tx, ty,
		tw, th,
		g_PolygonRot,g_width,g_height);

}

