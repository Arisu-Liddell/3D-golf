#include "Map.h"
#include "directx.h"
#include "Camera.h"
#include "Field.h"
#include "Ball.h"
#include "sprite.h"

static ID3D11Texture2D* g_MapTexture = NULL;
static ID3D11RenderTargetView* g_MapRTV = NULL;
static ID3D11ShaderResourceView* g_MapSRV = NULL;

static ID3D11Texture2D* g_MapDepthTexture = NULL;
static ID3D11DepthStencilView* g_MapDSV = NULL;


void MapInitialize(void)
{
    // レンダーテクスチャ作成
    {
        D3D11_TEXTURE2D_DESC td = {};
        td.Width = 256;
        td.Height = 256;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_B8G8R8X8_UNORM;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = 0;
        td.MiscFlags = 0;

        DirectXGetDevice()->CreateTexture2D(&td, NULL, &g_MapTexture);

        DirectXGetDevice()->CreateShaderResourceView(
            g_MapTexture,
            NULL,
            &g_MapSRV
        );

        DirectXGetDevice()->CreateRenderTargetView(
            g_MapTexture,
            NULL,
            &g_MapRTV
        );
    }
    // デプスバッファ作成
    {
        D3D11_TEXTURE2D_DESC td = {};
        td.Width = 256;
        td.Height = 256;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        td.CPUAccessFlags = 0;
        td.MiscFlags = 0;
        DirectXGetDevice()->CreateTexture2D(&td, NULL, &g_MapDepthTexture);
        DirectXGetDevice()->CreateDepthStencilView(
            g_MapDepthTexture,
            NULL,
            &g_MapDSV
        );
	}   
}


void MapFinalize(void)
{
    SAFE_RELEASE(g_MapTexture);
    SAFE_RELEASE(g_MapRTV);
    SAFE_RELEASE(g_MapSRV)

    SAFE_RELEASE(g_MapDepthTexture);
	SAFE_RELEASE(g_MapDSV)
}

void MapUpdate(void)
{
}

void MapDraw(void)
{
    //レンダーターゲット設定
    DirectXGetDeviceContext()->OMSetRenderTargets(
        1,
        &g_MapRTV,
        g_MapDSV
	);

	//ビューポート設定
    D3D11_VIEWPORT vp;
    vp.Width = 256;
    vp.Height = 256;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DirectXGetDeviceContext()->RSSetViewports(1, &vp);

    //クリア
	float clearColor[4] = { 0.8f,0.2f,0.2f,1.0f };
    DirectXGetDeviceContext()->ClearRenderTargetView(
        g_MapRTV,
        clearColor
	);
    DirectXGetDeviceContext()->ClearDepthStencilView(
        g_MapDSV,
        D3D11_CLEAR_DEPTH,
        1.0f,
        0
	);

    //ここで3Dオブジェクト描画
    CameraDrawMap();
    FieldDraw();
    BallDraw();

    //ビューポート設定
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DirectXGetDeviceContext()->RSSetViewports(1, &vp);

	//レンダーターゲット設定
    SetRenderTarget();

    //スプライト描画
    DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &g_MapSRV); //テクスチャの設定

	SpriteDraw(SCREEN_WIDTH -170, SCREEN_HEIGHT / 2, 256.0f, 256.0f,
        0.0f, 0.0f, 1.0f, 1.0f);
}
