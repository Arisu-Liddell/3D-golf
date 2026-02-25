#include "SoundInstance.h"
#include "Sound.h"
#include <cstring>

using namespace DirectX;

SoundInstance::SoundInstance(Sound& owner, int soundId)
    : m_Owner(owner)
    , m_SoundId(soundId)
{
}

SoundInstance::~SoundInstance()
{
    Stop();
}

bool SoundInstance::Play(int loopCount)
{
    if (!CreateVoice_()) return false;
    SubmitBufferAndStart_(loopCount);
    return true;
}

bool SoundInstance::Play3D(int loopCount, float emitterX, float emitterY, float emitterZ)
{
    if (!CreateVoice_()) return false;
    SubmitBufferAndStart_(loopCount);

    //3D適用
    Apply3D(emitterX, emitterY, emitterZ);
    return true;
}

void SoundInstance::Update3D(float emitterX, float emitterY, float emitterZ)
{
    if (!m_Voice) return;
    Apply3D(emitterX, emitterY, emitterZ);
}

void SoundInstance::Stop()
{
    DestroyVoice_();
}
void SoundInstance::SetVolume(float volume)
{
    if (!m_Voice) return;

    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    m_Voice->SetVolume(volume);
}
bool SoundInstance::CreateVoice_()
{
    // soundIdチェック
    if (!m_Owner.IsValidSoundId(m_SoundId)) return false;

    // 既に作っていたら一旦破棄
    DestroyVoice_();

    const WAVEFORMATEX* wfx = m_Owner.GetWaveFormat(m_SoundId);
    if (!wfx) return false;

    HRESULT hr = m_Owner.GetXAudio2()->CreateSourceVoice(&m_Voice, wfx);
    if (FAILED(hr) || !m_Voice)
    {
        m_Voice = nullptr;
        return false;
    }

    XAUDIO2_VOICE_DETAILS vd{};
    m_Voice->GetVoiceDetails(&vd);
    m_SrcChannels = vd.InputChannels;

    return true;
}

void SoundInstance::DestroyVoice_()
{
    if (!m_Voice) return;

    m_Voice->Stop(0);
    m_Voice->FlushSourceBuffers();
    m_Voice->DestroyVoice();
    m_Voice = nullptr;

    m_MatrixCoefficients.clear();
    m_SrcChannels = 1;
}

void SoundInstance::SubmitBufferAndStart_(int loopCount)
{
    if (!m_Voice) return;

    XAUDIO2_BUFFER buffer{};
    buffer.AudioBytes = m_Owner.GetAudioSize(m_SoundId);
    buffer.pAudioData = m_Owner.GetAudioData(m_SoundId);
    buffer.Flags = XAUDIO2_END_OF_STREAM;

    if (loopCount < 0) loopCount = XAUDIO2_LOOP_INFINITE;
    buffer.LoopCount = loopCount;

    // 再利用時
    m_Voice->Stop(0);
    m_Voice->FlushSourceBuffers();
   
    m_Voice->SubmitSourceBuffer(&buffer);
    m_Voice->Start(0);
}

void SoundInstance::Apply3D(float emitterX, float emitterY, float emitterZ)
{
    if (!m_Voice) return;

    const UINT32 dstChannels = m_Owner.GetDstChannels();

    // エミッタ
    X3DAUDIO_EMITTER emitter{};
    emitter.OrientFront = { 0.0f, 0.0f, 1.0f };
    emitter.OrientTop = { 0.0f, 1.0f, 0.0f };
    emitter.Position = { emitterX, emitterY, emitterZ };
    emitter.Velocity = { 0.0f, 0.0f, 0.0f };
    emitter.ChannelCount = m_SrcChannels;
    emitter.CurveDistanceScaler = 1.0f;
    emitter.DopplerScaler = 1.0f;

    // ステレオ入力時の方位
    static float s_azimuthsStereo[2] = { -X3DAUDIO_PI / 2.0f, X3DAUDIO_PI / 2.0f };
    if (m_SrcChannels == 2)
        emitter.pChannelAzimuths = s_azimuthsStereo;

    // 行列係数確保
    m_MatrixCoefficients.resize(static_cast<size_t>(m_SrcChannels) * dstChannels);

    X3DAUDIO_DSP_SETTINGS dsp{};
    dsp.SrcChannelCount = m_SrcChannels;
    dsp.DstChannelCount = dstChannels;
    dsp.pMatrixCoefficients = m_MatrixCoefficients.data();

    X3DAudioCalculate(
        *m_Owner.GetX3DHandle(),
        m_Owner.GetListener(),
        &emitter,
        X3DAUDIO_CALCULATE_MATRIX,
        &dsp
    );

    // 適用
    m_Voice->SetOutputMatrix(
        nullptr,
        m_SrcChannels,
        dstChannels,
        m_MatrixCoefficients.data()
    );
}
