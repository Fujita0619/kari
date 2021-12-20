#ifndef _GLWAVE_H_
#define	_GLWAVE_H_
//******************************************************************************
//
//
//		Wave�t�@�C��
//
//
//******************************************************************************

#include <windows.h>
#include <mmsystem.h>
#include <vector>
using namespace std;

//==============================================================================
//
//		���\�[�X
//
//==============================================================================

//// Wave�t�@�C��
//class GLWave
//{
//private:
//	WAVEFORMATEX	m_wfx;				// �t�H�[�}�b�g
//	DWORD			m_dataSize;			// �f�[�^�T�C�Y
//	BYTE*			m_pPCM;				// PCM�f�[�^
//public:
//	GLWave();
//	~GLWave();
//
//	void load(char *filename = NULL);
//private:
//	void loadPCM(HMMIO hmmio);
//public:
//	void release();
//	WAVEFORMATEX	getWfx() { return m_wfx; }
//	DWORD			getDataSize() { return m_dataSize; }
//	BYTE*			getPCM() { return m_pPCM; }
//};

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

#include <map>
using namespace std;

// Wave�t�@�C���Ǘ��N���X
class GLWaveManager
{
private:
	//vector<GLWave>		m_resource;
	map<char*, WAVE_DATA*>		m_resource;
public:
	static GLWaveManager* getInstance()
	{
		static GLWaveManager instance;
		return &instance;
	}

	void Init();
	void Uninit();

	//void Resize(int n);
	void Load(LOAD_WAVE* pLoad);
	void ReleaseAll();

	//GLWave* getResource(int i);
};

#define pGLWaveManager		(GLWaveManager::getInstance())

//******************************************************************************
#endif // !_GLWAVE_H_