//******************************************************************************
//
//
//		�T�E���h�֘A����
//
//
//******************************************************************************

//------< include >-------------------------------------------------------------

#include "GLAll.h"
#include <xaudio2.h>
#include <vector>
using namespace std;

//------< extern >--------------------------------------------------------------
//------< define >--------------------------------------------------------------

#define SE_MAX			0x100				// SE�̓����Đ��\��
#define QUEUE_SIZE		0x100				// �����L���[�̃T�C�Y

#define TEMPOUP_RATE	1.1f				// �e���|�A�b�v��

//------< structure >-----------------------------------------------------------

//--------------------------------
// XA_Voice�N���X
//--------------------------------
class XA_Voice
{
protected:
	IXAudio2SourceVoice*	m_pVoice;

public:
	XA_Voice();
	virtual ~XA_Voice();

	//virtual void Play(GLWave *pWave, bool loop = false);
	virtual void Play(WAVE_DATA* pWave, bool loop = false);
	virtual void Stop();

	void Pause();
	void Resume();

	float GetVolume();
	void SetVolume(float volume);

	float GetFrequency();
	void SetFrequency(float ratio);

	bool isPlaying();
};

//------< data >----------------------------------------------------------------
//------< work >----------------------------------------------------------------

IXAudio2*				m_pXAudio2;
static IXAudio2MasteringVoice*	m_pMasteringVoice;

static vector<XA_Voice>			m_se;

static XA_Voice					m_bgm;
static int						m_fadeTime;					// �t�F�[�h�A�E�g���ԁi0�F�t�F�[�h�A�E�g�Ȃ��j
static bool						m_pause;

static XA_Voice					m_voice;					// �����p�{�C�X
//static int						m_voice_queue[QUEUE_SIZE];	// �T�E���h�L���[
static WAVE_DATA*				m_voice_queue[QUEUE_SIZE];	// �T�E���h�L���[
static int						m_voice_read;				// �ǂݏo���|�C���^
static int						m_voice_write;				// �������݃|�C���^

static WAVE_DATA*				m_prevSE;					// ���O�ɍĐ����ꂽSE
		
//------< prototype >-----------------------------------------------------------
//==============================================================================
//
//		�T�E���h�V�X�e��
//
//==============================================================================

// �����ݒ�
void GLSound::Init()
{
	// �ϐ�������
	m_fadeTime = 0;
	m_pause = false;

	m_voice_read = 0;
	m_voice_write = 0;

	m_prevSE = nullptr;

	ResizeSE(SE_MAX);

	// Wave�̐ݒ�
	pGLWaveManager->Init();

	// XAudio2�̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	XAudio2Create(&m_pXAudio2, 0);
	m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
}

//==============================================================================

// �I������
void GLSound::Uninit()
{
	// �{�C�X�̒�~
	StopSound();

	// XAudio2�̏I������
	if (m_pMasteringVoice) m_pMasteringVoice->DestroyVoice();
	if (m_pXAudio2) m_pXAudio2->Release();
	CoUninitialize();

	// Wave�̉��
	pGLWaveManager->Uninit();
}

//==============================================================================

// �T�E���h�X�V����
void GLSound::Update()
{
	// �T�E���h�L���[����
	if (m_voice_read != m_voice_write) {						// �L���[�Ƀf�[�^���c���Ă���Ƃ�
		if (!m_voice.isPlaying()) {
			//WAVE_DATA *pWave = pGLWaveManager->getResource(m_voice_queue[m_voice_read]);
			WAVE_DATA *pWave = m_voice_queue[m_voice_read];
			m_voice.Play(pWave);
			m_voice_read = (m_voice_read + 1) % QUEUE_SIZE;		// ���[�h�|�C���^��i�߂�
		}
	}

	if (!m_pause) {
		// �t�F�[�h�A�E�g�`�F�b�N
		if (m_fadeTime) {
			m_bgm.SetVolume(m_bgm.GetVolume() - 1.0f / m_fadeTime);
			if (m_bgm.GetVolume() <= 0.0f) GLSound::StopBGM();
		}
	}

	m_prevSE = nullptr;
}

//==============================================================================
//
//		�T�E���h����
//
//==============================================================================

// �S�T�E���h��~
void GLSound::StopSound()
{
	StopSE();
	StopBGM();
	StopVoice();
}

// �|�[�Y
void GLSound::PauseSound()
{
	StopSE();
	StopVoice();
	m_bgm.Pause();
	m_pause = true;
}

// �|�[�Y����
void GLSound::ResumeSound()
{
	m_bgm.Resume();
	m_pause = false;
}

//==============================================================================
//
//		Wave
//
//==============================================================================

// Wave���̕ύX
//void GLSound::ResizeWave(int n)
//{
//	pGLWaveManager->Resize(n);
//}

// Wave�̃��[�h
void GLSound::LoadWave(LOAD_WAVE* pLoad)
{
	pGLWaveManager->Load(pLoad);
}

// Wave�̉��
void GLSound::ReleaseWave()
{
	pGLWaveManager->ReleaseAll();
}

//==============================================================================
//
//		SE
//
//==============================================================================

// SE���̕ύX
void GLSound::ResizeSE(int n)
{
	StopSE();
	m_se.resize(n);
}

// SE�̍Đ�
void GLSound::PlaySE(WAVE_DATA& wave)
{
	//GLWave* pWave = pGLWaveManager->getResource(wave);
	//if (!pWave) return;

	if (m_prevSE == &wave) return;		// ���O�Ɠ���SE�̂Ƃ��͍Đ����Ȃ�

	for (auto& se : m_se) {
		if (!se.isPlaying()) {
			se.Play(&wave, false);
			m_prevSE = &wave;
			break;
		}
	}
}

// SE�̒�~
void GLSound::StopSE()
{
	for (auto& se : m_se) se.Stop();
}

//==============================================================================
//
//		BGM
//
//==============================================================================

// BGM�̍Đ�
void GLSound::PlayBGM(WAVE_DATA& wave, bool loop)
{
	StopBGM();

	//GLWave	*pWave = pGLWaveManager->getResource(wave);
	m_bgm.Play(&wave, loop);
}

// BGM�̒�~
void GLSound::StopBGM()
{
	m_bgm.Stop();
	m_fadeTime = 0;
	m_pause = false;
}

// �t�F�[�h�A�E�g
void GLSound::FadeoutBGM(int fadeTime)
{
	m_fadeTime = fadeTime;
}

// �e���|�A�b�v
void GLSound::TempoupBGM()
{
	m_bgm.SetFrequency(m_bgm.GetFrequency() * TEMPOUP_RATE);
}

//==============================================================================
//
//		����
//
//==============================================================================

// �����̍Đ��i�L���[�ւ�PUSH�j
void GLSound::PlayVoice(WAVE_DATA& wave)
{
	int	next = (m_voice_write + 1) % QUEUE_SIZE;
	if (next == m_voice_read) return;

	m_voice_queue[m_voice_write] = &wave;			// �L���[��Wave��ۑ�
	m_voice_write = next;							// ���C�g�|�C���^��i�߂�
}

// �����̒�~�i�L���[��FLUSH�j
void GLSound::StopVoice()
{
	m_voice.Stop();
	m_voice_read = m_voice_write;					// �L���[�̒�~
}

//==============================================================================
//
//		XA_Voice�N���X
//
//==============================================================================

// �R���X�g���N�^
XA_Voice::XA_Voice() : m_pVoice(0)
{
}

// �f�X�g���N�^
XA_Voice::~XA_Voice()
{
	Stop();
}

//==============================================================================

// �Đ�
void XA_Voice::Play(WAVE_DATA *pWave, bool loop)
{
	Stop();

	auto wfx = pWave->getWfx();
	m_pXAudio2->CreateSourceVoice(&m_pVoice, &wfx);

	XAUDIO2_BUFFER		buffer;
	ZeroMemory(&buffer, sizeof(buffer));

	buffer.AudioBytes = pWave->getDataSize();
	buffer.pAudioData = pWave->getPCM();
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	buffer.LoopCount = (loop) ? XAUDIO2_LOOP_INFINITE : 0;

	m_pVoice->SubmitSourceBuffer(&buffer);
	m_pVoice->Start(0);
}

// ��~
void XA_Voice::Stop()
{
	if (m_pVoice) {
		m_pVoice->Stop(0);
		m_pVoice->FlushSourceBuffers();
		m_pVoice->DestroyVoice();
		m_pVoice = nullptr;
	}
}

//==============================================================================

// �ꎞ��~
void XA_Voice::Pause()
{
	if (m_pVoice) m_pVoice->Stop(0);
}

// �ĊJ
void XA_Voice::Resume()
{
	if (m_pVoice) m_pVoice->Start(0);
}

//==============================================================================

// �{�����[���̎擾
float XA_Voice::GetVolume()
{
	float volume = 0.0f;
	if (m_pVoice) m_pVoice->GetVolume(&volume);
	return volume;
}

// �{�����[���̐ݒ�
void XA_Voice::SetVolume(float volume)
{
	if (m_pVoice) {
		if (volume < 0.0f) volume = 0.0f;
		if (volume > 1.0f) volume = 1.0f;
		m_pVoice->SetVolume(volume);
	}
}

//==============================================================================

// �s�b�`�̎擾
float XA_Voice::GetFrequency()
{
	float ratio = 1.0f;
	if (m_pVoice) m_pVoice->GetFrequencyRatio(&ratio);
	return ratio;
}

// �s�b�`�̐ݒ�
void XA_Voice::SetFrequency(float ratio)
{
	if (m_pVoice) {
		if (ratio < XAUDIO2_MIN_FREQ_RATIO) ratio = XAUDIO2_MIN_FREQ_RATIO;
		if (ratio > XAUDIO2_DEFAULT_FREQ_RATIO) ratio = XAUDIO2_DEFAULT_FREQ_RATIO;
		m_pVoice->SetFrequencyRatio(ratio);
	}
}

//==============================================================================

// �Đ����`�F�b�N
bool XA_Voice::isPlaying()
{
	if (!m_pVoice) return false;

	XAUDIO2_VOICE_STATE	state;
	m_pVoice->GetState(&state);
	return (state.BuffersQueued != 0);
}

//******************************************************************************
