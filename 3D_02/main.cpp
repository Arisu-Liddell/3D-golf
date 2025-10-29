#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> //Windows.h = WindowsAPIのヘッダファイル。MessageBoxなどの関数を使うのに必要
#include <mmsystem.h>
#include "directx.h"
#include "debug_ostream.h"
#include "shader.h"
#include "title.h"
#include "texture.h"
#include "sprite.h"
#include "keyboard.h"
#include "score.h"
#include "polygon.h"
#include "game.h"
#include "main.h"
#include "result.h"
#pragma comment(lib, "winmm.lib")

//////////////////////////////////
// ウィンドウ情報
//////////////////////////////////

static constexpr char WINDOW_CLASS[] = "GameWindow";
static constexpr char TITLE[] = "ウィンドウ表示";

#ifdef _DEBUG
int g_CountFPS = 0;
char g_DebugStr[2048] = "ウィンドウ表示";//デバッグ表示用
#endif

static SCENE g_scene;
static SCENE g_next;
static int g_TransitionTexture;
static bool g_Transition;
static float g_TransitionPositionY;

///////////////////////////////////////////
// ウィンドウプロシージャのプロトタイプ宣言
///////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM, LPARAM lParam);

//更新処理と描画処理のプロトタイプ宣言
void Initialize(HWND hWnd);
void Update(void);
void Draw(void);
void Finalize(void); //終了処理のプロトタイプ宣言


//メイン関数
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウィンドウクラスの登録
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX); //構造体のサイズを指定する
	wcex.lpfnWndProc = WndProc; //デフォルトのウィンドウプロシージャを指定する
	wcex.hInstance = hInstance; //アプリケーションのインスタンスを指定する
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); //アイコンを指定する
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); //カーソルを指定する
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));; //背景色を指定する
	wcex.lpszMenuName = nullptr; //メニュー名を指定しない
	wcex.lpszClassName = WINDOW_CLASS; //クラス名を指定する
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	RegisterClassEx(&wcex); //ウィンドウクラスを登録する

	//クライアント領域のサイズを持った矩形（左からleft,top,right,bottom）を作成する
	RECT windou_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; //ウィンドウのサイズを指定するSCREENHEIGHTに変える
	//ウィンドウのスタイル
	DWORD style = WS_OVERLAPPED| WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX;
	//指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&windou_rect, style, FALSE); //ウィンドウの矩形を調整する

	//メインウィンドウの作成
	HWND hWnd = CreateWindow(WINDOW_CLASS, TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,0, CW_USEDEFAULT,0,nullptr,nullptr,hInstance,nullptr); 

	//初期化
	Initialize(hWnd); //初期化処理を呼び出す

	ShowWindow(hWnd, nCmdShow); //ウィンドウを表示する
	UpdateWindow(hWnd); //ウィンドウを更新する

	//時間計測用
	DWORD dwExecLastTime; //現在の時間を取得
	DWORD dwFPSLastTime; //前回の時間を取得
	DWORD dwCurrentTime; //時間計測用のカウンタ
	DWORD dwFrameCount; //フレームカウント

	//フレームカウント初期化
	timeBeginPeriod(1); //分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime(); //システム時刻をミリ秒単位で取得
	dwCurrentTime  = dwFrameCount = 0;

	//メッセージループ
	MSG msg;
	do {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { //メッセージがある間ループする
			TranslateMessage(&msg); //メッセージを変換する
			DispatchMessage(&msg);
		}
		else {
			dwCurrentTime = timeGetTime(); //現在の時間を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)
			{ 
#ifdef _DEBUG //デバッグ表示用のFPS表示
				g_CountFPS = dwFrameCount; //フレームカウントをFPSに設定
#endif
				dwFPSLastTime = dwCurrentTime; //測定時間を保存
				dwFrameCount = 0; //フレームカウントをリセット
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000/60)) //1/60秒経過したら
			{
				dwExecLastTime = dwCurrentTime; //現在の時間を保存

#ifdef _DEBUG //デバッグ表示用のFPS表示
				wsprintf(g_DebugStr, TITLE);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)],"FPS:%d",g_CountFPS);
				SetWindowText(hWnd, g_DebugStr); //デバッグ出力
#endif
			Update();
			Draw();

			dwFrameCount++; //フレームカウントをインクリメント
			}
		}
	} while (msg.message != WM_QUIT); //WM_QUITメッセージが来るまでループする

	//終了処理
	Finalize(); //終了処理を呼び出す

	return (int)msg.wParam; //メッセージのwParamを返す
}
////////////////////////////////////////////////
//   ウィンドウプロシージャプロトタイプ宣言
////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY: //ウィンドウが破棄されたとき
		PostQuitMessage(0); //メッセージループを終了する
		break;

	case WM_KEYDOWN:
		//if (wParam == VK_ESCAPE) //ESCキーが押されたとき
		//	{
		//		SendMessage(hWnd, WM_CLOSE, 0, 0); //メッセージループを終了する
		//	}

	case WM_SYSKEYDOWN: //左クリックされたとき
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(message, wParam, lParam); //キーボードの状態を取得する
			break;

	case WM_CLOSE: //ウィンドウが閉じられたとき
	{
			int ret = MessageBox(hWnd, "終了しますか？", "ゲーム", MB_OKCANCEL); //
			if (ret == IDOK)
			{
				DestroyWindow(hWnd); //ウィンドウを破棄する
			}
		break;
	}
				
	default:
		return DefWindowProc(hWnd, message, wParam, lParam); 
	}
	return 0;
}


void Initialize(HWND hWnd)
{
	// 	//Com初期化
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED); //COMライブラリの初期化
	//キーボードの初期化処理
	Keyboard_Initialize();
	//DirectXの初期化処理
	DirectXInitialize(hWnd);
	//シェーダーの初期化
	Shader_Initialize(DirectXGetDevice(), DirectXGetDeviceContext());//NULLを突っ込む
	//テクスチャ初期化
	TextureInitialize(DirectXGetDevice());
	//スプライト初期化
	SpriteInitialize();
	//ポリゴンの初期化
	PolygonInitialize();

	g_TransitionTexture = TextureLoad(L"asset\\texture\\bg_01.png");

	g_scene = SCENE_NONE;

	g_Transition = false;

	SetScene(SCENE_TITLE);


}

void Update(void)
{
	//更新処理
	switch (g_scene)
	{
	case SCENE_TITLE:
		TitleUpdate();
		break;
	case SCENE_GAME:
		GameUpdate();
		break;
	case SCENE_RESULT:
		ResultUpdate();
		break;
	default:
		break;
	}
	if (g_Transition == true)
	{
		g_TransitionPositionY += 10.0f;
		if (g_TransitionPositionY > SCREEN_HEIGHT / 2)
		{
			g_TransitionPositionY = SCREEN_HEIGHT / 2;

			SetScene(g_next);

			g_Transition = false;
		}
	}
		Keyboard_Update();

}
void Draw(void)
{
	Clear();

	switch (g_scene)
	{
	case SCENE_TITLE:
		TitleDraw();
		break;
	case SCENE_GAME:
		GameDraw();
		break;
	case SCENE_RESULT:
		ResultDraw();
		break;
	}

	if (g_Transition == true)
	{
		//テクスチャの設定
		ID3D11ShaderResourceView* texture = GetTexture(g_TransitionTexture); //テクスチャの取得
		DirectXGetDeviceContext()->PSSetShaderResources(0, 1, &texture); //テクスチャの設定

		SpriteDraw(SCREEN_WIDTH / 2, g_TransitionPositionY, SCREEN_WIDTH, SCREEN_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f);
	}

	Present();
}


void Finalize(void)
{


	//終了処理
	PolygonFinalize();
	//スプライトの終了処理
	SpriteFinalize();
	//シェーダーの終了処理
	Shader_Finalize();

	//テクスチャの終了処理
	TextureFinalize();

	//DirectXの終了処理
	DirectXFinalize();

	//Com終了
	CoUninitialize(); //COMライブラリの終了

}

void SetScene(SCENE scene)
{
	////やけくそコード群
	
	switch (g_scene)
	{
	case SCENE_TITLE:
		TitleFinalize();
		GameFinalize();
		ResultFinalize();
		break;
	case SCENE_GAME:
		TitleFinalize();
		GameFinalize();
		ResultFinalize();
		break;
	case SCENE_RESULT:
		TitleFinalize();
		GameFinalize();
		ResultFinalize();
		break;
	}
	g_scene = scene;
	switch (g_scene)
	{
	case SCENE_TITLE:
		GameInitialize();
		ResultInitialize();
		TitleInitialize();
		break;
	case SCENE_GAME:
		GameInitialize();
		ResultInitialize();
		TitleInitialize();
		break;
	case SCENE_RESULT:
		GameInitialize();
		ResultInitialize();
		TitleInitialize();
		break;
	}
}

void Transition(SCENE Next)
{
	g_next = Next;

	g_Transition = true;

	g_TransitionPositionY = -SCREEN_HEIGHT / 2;
}
/*
winmain.cpp = 最初に呼ばれる関数　導入関数
lpCmdLine = コマンドライン引数 アプリケーションを起動する時に指定する引数
hInstance = アプリケーションのインスタンスを指定する 引数の型を指定する 
_in_ = 引数の型を指定する 意味をつけるための仕組み　visual studioで指定するためのやつ　拡張？

#include <Windows.h> = WindowsAPIを使うためのインクルード
Windows.h = WindowsAPIのヘッダファイル。MessageBoxなどの関数を使うのに必要
bitflag 
パイプ記号　｜　SHIFT　＋　￥　＝　｜
ビットに引数をしていすることで機能を実装できる？
*/