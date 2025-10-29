#include <d3d11.h>
#include "directx.h"

#pragma comment (lib, "d3d11.lib")//DirectX11のスタティックリンクライブラリをリンクする

static ID3D11Device* g_Device = NULL; //デバイス
static ID3D11DeviceContext* g_DeviceContext = NULL; //デバイスコンテキスト
static IDXGISwapChain* g_SwapChain = NULL; //スワップチェイン
static ID3D11Texture2D* g_DepthStencilTexture = NULL; //深度ステンシルバッファ
static ID3D11RenderTargetView* g_RenderTargetView = NULL; //レンダーターゲットビュー
static ID3D11DepthStencilView* g_DepthStencilView = NULL; //深度ステンシルビュー

static ID3D11RasterizerState* g_RasterizerState = NULL; //ビューポート
static ID3D11BlendState* g_BlendState = NULL; //ブレンドステート
static ID3D11DepthStencilState* g_DepthStencilStateDepthDisable = NULL; //深度ステンシルステート無効
static ID3D11DepthStencilState* g_DepthStencilStateDepthEnable  = NULL; //深度ステンシルステート無効



void DirectXInitialize(HWND hWnd)
{
	//デバイス、スパップチェイン、コンテキスト生成
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1; //バッファ数
	sd.BufferDesc.Width = 1600; //SCREENWIDTH //ウィンドウ幅
	sd.BufferDesc.Height = 900; //SCREENHEIGHT//ウィンドウ高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //バッファフォーマット
	sd.BufferDesc.RefreshRate.Numerator = 60; //バッファ使用法
	sd.BufferDesc.RefreshRate.Denominator = 1; //バッファ使用法
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //バッファ使用法
	sd.OutputWindow = hWnd; //ウィンドウハンドル
	sd.SampleDesc.Count = 1; //マルチサンプリング数
	sd.SampleDesc.Quality = 0; //マルチサンプリング品質
	sd.Windowed = TRUE; //ウィンドウモード

	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&g_SwapChain,
		&g_Device,
		&feature_level,
		&g_DeviceContext);

	if (FAILED(hr))
	{
	//	MessageBox(hWnd, "DirectXの初期化に失敗しました", "エラー", MB_OK);
		return;
	}

	//レンダーターゲットビューの生成
	ID3D11Texture2D* pBackBuffer = NULL;
	g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_RenderTargetView);
	pBackBuffer->Release();

	//深度ステンシルテクスチャーの生成
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	g_Device->CreateTexture2D(&td, NULL, &g_DepthStencilTexture);

	//深度ステンシルビューの生成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	g_Device->CreateDepthStencilView(g_DepthStencilTexture, &dsvd, &g_DepthStencilView);

	//デバイスコンテキストへレンダーターゲットビューと深度ステンシルビューを設定
	g_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);

	//ビューポート設定 設定を変えるとミニマップと科病ができる
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)1600;//1600
	vp.Height = (FLOAT)900;//900
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_DeviceContext->RSSetViewports(1, &vp);

	//ラスタライザーステート設定
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;
	g_Device->CreateRasterizerState(&rd, &g_RasterizerState);

	//ブレンドステート設定
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = FALSE;
	bd.RenderTarget[0].BlendEnable = TRUE;

	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;


	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_Device->CreateBlendState(&bd, &g_BlendState);

	g_DeviceContext->OMSetBlendState(g_BlendState, blendFactor, 0xffffffff);

	//深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = FALSE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

		dsd.StencilEnable = FALSE;
		g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthDisable);
		dsd.DepthEnable = TRUE;
		g_Device->CreateDepthStencilState(&dsd, &g_DepthStencilStateDepthEnable);
}
void DirectXFinalize(void)
{
	SAFE_RELEASE(g_DepthStencilStateDepthDisable);
	SAFE_RELEASE(g_BlendState);
	SAFE_RELEASE(g_RasterizerState);

	SAFE_RELEASE(g_DepthStencilView);
	SAFE_RELEASE(g_DepthStencilTexture);
	SAFE_RELEASE(g_RenderTargetView);
	SAFE_RELEASE(g_SwapChain);
	SAFE_RELEASE(g_DeviceContext);
	SAFE_RELEASE(g_Device);
}

ID3D11Device* DirectXGetDevice(void)
{
	return g_Device;
}



ID3D11DeviceContext* DirectXGetDeviceContext(void)
{
	return g_DeviceContext;
}

void Clear(void)
{
	//画面クリア
	float clear_color[4] = { 0.2f, 0.2f, 0.8f, 1.0f };
	g_DeviceContext->ClearRenderTargetView(g_RenderTargetView, clear_color);
	g_DeviceContext->ClearDepthStencilView(g_DepthStencilView, D3D11_CLEAR_DEPTH ,1.0f, 0);
}

void Present(void)
{
	//画面更新
	g_SwapChain->Present(0, 0);
}




void SetDepthEnable(bool DepthEnable)
{
	if (DepthEnable)
	{
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthEnable, NULL);
	}
	else 
	{
		g_DeviceContext->OMSetDepthStencilState(g_DepthStencilStateDepthDisable, NULL);
	}
}