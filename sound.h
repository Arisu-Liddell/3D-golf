#ifndef SOUND_H
#define SOUND_H
#define NOMINMAX
#pragma comment(lib, "Xaudio2.lib")

#include <windows.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <cmath>

class Sound
{
public:
	Sound(HWND hWnd);
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;
	~Sound();

	int LoadSound(const char* pFilename);
	void PlaySound(int index, int loopCount);
	void SetVolume(int index, float vol);
	void StopSound(int index);
	void StopSoundAll(void);
	void SetListenerPosition(float x, float y, float z);
	void SetListenerTransform(float px, float py, float pz,float fx, float fy, float fz,float ux, float uy, float uz);
	void PlaySound3D(int index, int loopCount, float emitterX, float emitterY, float emitterZ);
	void UpdateSound3D(int index, float emitterX, float emitterY, float emitterZ);

	static Sound* GetInstance() { return s_pInstance; }


	// SoundInstance ‚©‚çŽg‚¤‚½‚ß‚ÌAPI
	IXAudio2* GetXAudio2() const { return g_pXAudio2; }
	const X3DAUDIO_HANDLE* GetX3DHandle() const { return &m_X3DInstance; }
	const X3DAUDIO_LISTENER* GetListener() const { return &m_Listener; }
	UINT32 GetDstChannels() const { return m_DstChannels; }

	bool IsValidSoundId(int id) const
	{
		return (id >= 0 && id < static_cast<int>(g_aSizeAudio.size()));
	}

	const WAVEFORMATEX* GetWaveFormat(int id) const
	{
		if (id < 0 || id >= static_cast<int>(g_aWaveFormat.size()))
			return nullptr;

		return &g_aWaveFormat[id];
	}

	const BYTE* GetAudioData(int id) const { return g_apDataAudio[id]; }
	DWORD GetAudioSize(int id) const { return g_aSizeAudio[id]; }

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2* g_pXAudio2{ nullptr };
	IXAudio2MasteringVoice* g_pMasteringVoice{ nullptr };

	std::vector<IXAudio2SourceVoice*> g_apSourceVoice;
	std::vector<BYTE*> g_apDataAudio;
	std::vector<DWORD> g_aSizeAudio;
	std::vector<std::string> g_SoundNames;
	std::vector<WAVEFORMATEX> g_aWaveFormat;

	float m_ListenerX{ 0.0f };
	float m_ListenerY{ 0.0f };
	float m_ListenerZ{ 0.0f };

	// --- X3DAudio —p ---
	X3DAUDIO_HANDLE   m_X3DInstance{};
	X3DAUDIO_LISTENER m_Listener{};
	DWORD             m_ChannelMask = 0;
	UINT32            m_DstChannels = 2;

	std::vector<float> m_MatrixCoefficients;
	static Sound* s_pInstance;
};

#endif
