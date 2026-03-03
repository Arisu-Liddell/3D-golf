#include "Sound.h"
#define MAX_SOUND_NUM 100
Sound* Sound::s_pInstance = nullptr;


Sound::Sound(HWND hWnd)
	:
	g_pMasteringVoice(NULL),
	m_ListenerX(0.0f),
	m_ListenerY(0.0f),
	m_ListenerZ(0.0f)
{
	HRESULT hr = 0;

	// COMライブラリの初期化ｐ
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
	{
		MessageBoxW(hWnd, L"COM初期化に失敗！", L"警告！", MB_ICONWARNING);
		return;
	}

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBoxW(hWnd, L"XAudio2オブジェクトの作成に失敗！", L"警告！", MB_ICONWARNING);
		// COMライブラリの終了処理
		CoUninitialize();
		return;
	}

	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBoxW(hWnd, L"マスターボイスの生成に失敗！", L"警告！", MB_ICONWARNING);

		if (g_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}
		CoUninitialize();
		return;
	}

	//X3DAudioの初期化
	XAUDIO2_VOICE_DETAILS mvDetails{};
	g_pMasteringVoice->GetVoiceDetails(&mvDetails);
	m_DstChannels = mvDetails.InputChannels;

	// スピーカー構成取得
	if (FAILED(g_pMasteringVoice->GetChannelMask(&m_ChannelMask)))
	{
		//ステレオ想定
		m_ChannelMask = SPEAKER_STEREO;
	}

	// インスタンス初期化
	X3DAudioInitialize(m_ChannelMask, X3DAUDIO_SPEED_OF_SOUND, m_X3DInstance);

	// Listenerの初期値
	m_Listener.OrientFront = { 0.0f, 0.0f, 1.0f };
	m_Listener.OrientTop = { 0.0f, 1.0f, 0.0f };
	m_Listener.Position = { 0.0f, 0.0f, 0.0f };
	m_Listener.Velocity = { 0.0f, 0.0f, 0.0f };
	s_pInstance = this;

}

Sound::~Sound()
{
	if (s_pInstance == this) s_pInstance = nullptr;
	// 全てのサウンドを停止する
	for (size_t nCntSound = 0; nCntSound < g_apSourceVoice.size(); nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();

			// オーディオデータの開放
			if (g_apDataAudio[nCntSound])
			{
				free(g_apDataAudio[nCntSound]);
			}
		}
	}

	//vectorをクリア
	g_apSourceVoice.clear();
	g_apDataAudio.clear();
	g_aSizeAudio.clear();
	g_SoundNames.clear();

	// マスターボイスの破棄
	if (g_pMasteringVoice) {
		g_pMasteringVoice->DestroyVoice();
		g_pMasteringVoice = NULL;
	}

	if (g_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

void Sound::SetListenerPosition(float x, float y, float z)
{
	m_ListenerX = x;
	m_ListenerY = y;
	m_ListenerZ = z;

	m_Listener.Position = { x, y, z };
}
void Sound::SetListenerTransform(
	float px, float py, float pz,
	float fx, float fy, float fz,
	float ux, float uy, float uz
) {
	m_Listener.Position = { px, py, pz };
	m_Listener.OrientFront = { fx, fy, fz };
	m_Listener.OrientTop = { ux, uy, uz };
}

int Sound::LoadSound(const char* pFilename)
{
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	//読み込まれているサウンド名を調べて、同名のものが
	//すでに読み込まれていたらその番号を返す
	for (size_t i = 0; i < g_SoundNames.size(); i++)
	{
		//サウンド名を比較
		if (g_SoundNames[i] == pFilename)
		{
			return static_cast<int>(i);
		}
	}

	if (g_SoundNames.size() >= MAX_SOUND_NUM)
	{
		MessageBoxW(NULL, L"最大読み込み数を超過", L"警告！", MB_ICONWARNING);
		return -1;
	}

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	int wideLength = MultiByteToWideChar(CP_UTF8, 0, pFilename, -1, nullptr, 0);
	std::wstring wideFilename(wideLength, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, pFilename, -1, &wideFilename[0], wideLength);

	hFile = CreateFileW(wideFilename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxW(NULL, L"サウンドデータファイルの生成に失敗！(1)", L"警告！", MB_ICONWARNING);
		return -1;
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		MessageBoxW(NULL, L"サウンドデータファイルの生成に失敗！(2)", L"警告！", MB_ICONWARNING);
		return -1;
	}

	HRESULT hr;

	// WAVEファイルのチェック
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"WAVEファイルのチェックに失敗！(1)", L"警告！", MB_ICONWARNING);
		return -1;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"WAVEファイルのチェックに失敗！(2)", L"警告！", MB_ICONWARNING);
		return -1;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBoxW(NULL, L"WAVEファイルのチェックに失敗！(3)", L"警告！", MB_ICONWARNING);
		return -1;
	}

	// フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"フォーマットチェックに失敗！(1)", L"警告！", MB_ICONWARNING);
		return -1;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"フォーマットチェックに失敗！(2)", L"警告！", MB_ICONWARNING);
		return -1;
	}

	// オーディオデータ読み込み
	DWORD audioSize = 0;
	hr = CheckChunk(hFile, 'atad', &audioSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"オーディオデータ読み込みに失敗！(1)", L"警告！", MB_ICONWARNING);
		return -1;
	}
	BYTE* audioData = (BYTE*)malloc(audioSize);
	if (!audioData)
	{
		MessageBoxW(NULL, L"メモリ割り当てに失敗", L"警告！", MB_ICONWARNING);
		return -1;
	}
	hr = ReadChunkData(hFile, audioData, audioSize, dwChunkPosition);
	if (FAILED(hr))
	{
		free(audioData);
		MessageBoxW(NULL, L"オーディオデータ読み込みに失敗！(2)", L"警告！", MB_ICONWARNING);
		return -1;
	}

	// ソースボイスの生成
	IXAudio2SourceVoice* sourceVoice = nullptr;
	hr = g_pXAudio2->CreateSourceVoice(&sourceVoice, &(wfx.Format));
	if (FAILED(hr))
	{
		free(audioData);
		MessageBoxW(NULL, L"ソースボイスの生成に失敗！", L"警告！", MB_ICONWARNING);
		return -1;
	}

	//読み込んだサウンド名を保存する
	g_apSourceVoice.push_back(sourceVoice);
	g_apDataAudio.push_back(audioData);
	g_aSizeAudio.push_back(audioSize);
	g_SoundNames.push_back(pFilename);
	g_aWaveFormat.push_back(wfx.Format);

	return static_cast<int>(g_SoundNames.size() - 1);
}

void Sound::PlaySound(int index, int loopCount)
{
	//検索範囲をチェックする
	if (index < 0 || index >= static_cast<int>(g_apSourceVoice.size()))
		return;

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[index];
	buffer.pAudioData = g_apDataAudio[index];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (loopCount < 0)
		loopCount = XAUDIO2_LOOP_INFINITE;
	buffer.LoopCount = loopCount;

	// 状態取得
	g_apSourceVoice[index]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[index]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[index]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[index]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[index]->Start(0);
}


// [PlaySound3D] 置き換えブロック（1行目から）
void Sound::PlaySound3D(int index, int loopCount,
	float emitterX, float emitterY, float emitterZ)
{
	if (index < 0 || index >= static_cast<int>(g_apSourceVoice.size()))
		return;

	XAUDIO2_VOICE_STATE xa2state{};
	g_apSourceVoice[index]->GetState(&xa2state);

	// --- 再生中でも「後から呼んだ方を鳴らす」ため、必ず入れ替える ---
	if (xa2state.BuffersQueued != 0)
	{
		g_apSourceVoice[index]->Stop(0);
		g_apSourceVoice[index]->FlushSourceBuffers();
	}

	XAUDIO2_BUFFER buffer{};
	buffer.AudioBytes = g_aSizeAudio[index];
	buffer.pAudioData = g_apDataAudio[index];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (loopCount < 0) loopCount = XAUDIO2_LOOP_INFINITE;
	buffer.LoopCount = loopCount;

	g_apSourceVoice[index]->SubmitSourceBuffer(&buffer);
	g_apSourceVoice[index]->Start(0);

	// ---- ここから下（X3DAudio計算～SetOutputMatrix）は今のままでOK ----
	XAUDIO2_VOICE_DETAILS srcDetails{};
	g_apSourceVoice[index]->GetVoiceDetails(&srcDetails);
	const UINT32 srcChannels = srcDetails.InputChannels;

	X3DAUDIO_EMITTER emitter{};
	emitter.OrientFront = { 0.0f, 0.0f, 1.0f };
	emitter.OrientTop = { 0.0f, 1.0f, 0.0f };
	emitter.Position = { emitterX, emitterY, emitterZ };
	emitter.Velocity = { 0.0f, 0.0f, 0.0f };
	emitter.ChannelCount = srcChannels;
	emitter.CurveDistanceScaler = 1.0f;
	emitter.DopplerScaler = 1.0f;

	static float s_azimuthsStereo[2] = { -X3DAUDIO_PI / 2.0f, X3DAUDIO_PI / 2.0f };
	if (srcChannels == 2) emitter.pChannelAzimuths = s_azimuthsStereo;

	m_MatrixCoefficients.resize(static_cast<size_t>(srcChannels) * m_DstChannels);
	X3DAUDIO_DSP_SETTINGS dsp{};
	dsp.SrcChannelCount = srcChannels;
	dsp.DstChannelCount = m_DstChannels;
	dsp.pMatrixCoefficients = m_MatrixCoefficients.data();

	X3DAudioCalculate(m_X3DInstance, &m_Listener, &emitter, X3DAUDIO_CALCULATE_MATRIX, &dsp);

	g_apSourceVoice[index]->SetOutputMatrix(nullptr, srcChannels, m_DstChannels, m_MatrixCoefficients.data());

}

void Sound::UpdateSound3D(int index, float emitterX, float emitterY, float emitterZ)
{
	if (index < 0 || index >= static_cast<int>(g_apSourceVoice.size()))
		return;

	XAUDIO2_VOICE_STATE state{};
	g_apSourceVoice[index]->GetState(&state);
	if (state.BuffersQueued == 0)
		return; // 鳴ってない音は更新しない

	// チャンネル数取得
	XAUDIO2_VOICE_DETAILS srcDetails{};
	g_apSourceVoice[index]->GetVoiceDetails(&srcDetails);
	const UINT32 srcChannels = srcDetails.InputChannels;

	// エミッタ
	X3DAUDIO_EMITTER emitter{};
	emitter.OrientFront = { 0,0,1 };
	emitter.OrientTop = { 0,1,0 };
	emitter.Position = { emitterX, emitterY, emitterZ };
	emitter.Velocity = { 0,0,0 };
	emitter.ChannelCount = srcChannels;
	emitter.CurveDistanceScaler = 1.0f;
	emitter.DopplerScaler = 1.0f;

	static float s_azimuthsStereo[2] = {
		-X3DAUDIO_PI / 2.0f,
		 X3DAUDIO_PI / 2.0f
	};
	if (srcChannels == 2)
		emitter.pChannelAzimuths = s_azimuthsStereo;

	m_MatrixCoefficients.resize(srcChannels * m_DstChannels);

	X3DAUDIO_DSP_SETTINGS dsp{};
	dsp.SrcChannelCount = srcChannels;
	dsp.DstChannelCount = m_DstChannels;
	dsp.pMatrixCoefficients = m_MatrixCoefficients.data();

	X3DAudioCalculate(
		m_X3DInstance,
		&m_Listener,
		&emitter,
		X3DAUDIO_CALCULATE_MATRIX,
		&dsp
	);

	g_apSourceVoice[index]->SetOutputMatrix(
		nullptr,
		srcChannels,
		m_DstChannels,
		m_MatrixCoefficients.data()
	);
}



void Sound::SetVolume(int index, float vol)
{
	//検索範囲をチェックする
	if (index < 0 || index >= static_cast<int>(g_apSourceVoice.size()))
		return;

	g_apSourceVoice[index]->SetVolume(vol);
}

void Sound::StopSound(int index)
{
	XAUDIO2_VOICE_STATE xa2state;
	//検索範囲をチェックする
	if (index < 0 || index >= static_cast<int>(g_apSourceVoice.size()))
		return;
	// 状態取得
	g_apSourceVoice[index]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[index]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[index]->FlushSourceBuffers();
	}
}

void Sound::StopSoundAll(void)
{
	// 一時停止
	for (size_t nCntSound = 0; nCntSound < g_apSourceVoice.size(); nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}


/*------------------------------------------------------------------------------
   WAVEフォーマットのチェック
------------------------------------------------------------------------------*/
HRESULT Sound::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		dwBytesRead = dwOffset;// 読み込みバイト数の更新
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

/*------------------------------------------------------------------------------
   WAVEフォーマットの読み込み
------------------------------------------------------------------------------*/
HRESULT Sound::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}