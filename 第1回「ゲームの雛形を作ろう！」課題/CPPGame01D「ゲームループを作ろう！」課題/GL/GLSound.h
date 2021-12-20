#ifndef _GLSOUND_H_
#define	_GLSOUND_H_
//******************************************************************************
//
//
//		Windows�֘A����
//
//
//******************************************************************************

namespace GLSound
{
	void Init();
	void Uninit();
	void Update();

	// �T�E���h����
	void StopSound();
	void PauseSound();
	void ResumeSound();
	// Wave
	//void ResizeWave(int n);
	void LoadWave(LOAD_WAVE* pLoad);
	void ReleaseWave();
	// SE
	void ResizeSE(int n);
	void PlaySE(WAVE_DATA& wave);
	void StopSE();
	// BGM
	void PlayBGM(WAVE_DATA& wave, bool loop = true);
	void StopBGM();
	void FadeoutBGM(int fadeTime);
	void TempoupBGM();
	// ����
	void PlayVoice(WAVE_DATA& wave);
	void StopVoice();
};

//******************************************************************************
#endif // !_GLSOUND_H_