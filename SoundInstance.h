#pragma once
#pragma comment(lib, "Xaudio2.lib")

#include <DirectXMath.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include <vector>

class Sound;

// 「同じWAVを複数同時に鳴らす」ための再生個体（Instance）
class SoundInstance
{
public:
    SoundInstance(Sound& owner, int soundId);
    ~SoundInstance();

    SoundInstance(const SoundInstance&) = delete;
    SoundInstance& operator=(const SoundInstance&) = delete;

    // 再生開始（loopCount: -1で無限）
    bool Play(int loopCount);

    // 3D再生開始（loopCount: -1で無限）
    bool Play3D(int loopCount, float emitterX, float emitterY, float emitterZ);

    // 3D位置更新（毎フレーム呼ぶ想定）
    void Update3D(float emitterX, float emitterY, float emitterZ);

    // 停止
    void Stop();

	// ボリューム設定（0.0f～1.0f）
    void SetVolume(float volume);

    // 生存確認（voiceが有効か）
    bool IsAlive() const { return (m_Voice != nullptr); }
    int  GetSoundId() const { return m_SoundId; }
    //ゲッター
    Sound* GetSound() const { return &m_Owner; }

private:
    bool CreateVoice_();
    void DestroyVoice_();
    void SubmitBufferAndStart_(int loopCount);
    void Apply3D(float emitterX, float emitterY, float emitterZ);

private:
    Sound& m_Owner;
    int    m_SoundId = -1;

    IXAudio2SourceVoice* m_Voice = nullptr;

    // 出力行列（srcChannels * dstChannels）
    std::vector<float> m_MatrixCoefficients;

    // 入力ch数（WAVのch）
    UINT32 m_SrcChannels = 1;
};
