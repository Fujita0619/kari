//******************************************************************************
//
//
//		Wave�t�@�C��
//
//
//******************************************************************************

#pragma comment ( lib, "winmm.lib" )

#include "GLAll.h"

//#define GLWAVE_MAX			0x100			// Wave�t�@�C���ő吔�i�f�t�H���g�j

//==============================================================================
//
//		���\�[�X
//
//==============================================================================

// �R���X�g���N�^
//GLWave::GLWave()
//{
//	ZeroMemory(this, sizeof(*this));
//}

//==============================================================================

//// �f�X�g���N�^
//GLWave::~GLWave()
//{
//	release();
//}

//==============================================================================

//// ���
//void GLWave::release()
//{
//	if (m_pPCM) {
//		delete[] m_pPCM;
//		m_pPCM = nullptr;
//	}
//}

//==============================================================================

//// ���[�h
//void GLWave::load(char* filename)
//{
//	release();
//	
//	HMMIO hmmio = mmioOpenA(filename, nullptr, MMIO_ALLOCBUF | MMIO_READ);
//	if (hmmio) {
//		loadPCM(hmmio);
//		mmioClose(hmmio, 0);
//	}
//}

//------------------------------------------------------------------------------

//// PCM�f�[�^�̃��[�h
//void GLWave::loadPCM(HMMIO hmmio)
//{
//	MMCKINFO		mainChunk;
//	MMCKINFO		subChunk;
//
//	// RIFF�`�����N��WAVE�^�C�v����
//	mainChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
//	if (mmioDescend(hmmio, &mainChunk, NULL, MMIO_FINDRIFF)) return;
//
//	// fmt�`�����N�̌���
//	subChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
//	if (mmioDescend(hmmio, &subChunk, &mainChunk, MMIO_FINDCHUNK)) return;
//
//	// �t�H�[�}�b�g�̓ǂݍ���
//	if (mmioRead(hmmio, (HPSTR)&m_wfx, sizeof(m_wfx)) != sizeof(m_wfx)) return;
//	if (m_wfx.wFormatTag != WAVE_FORMAT_PCM) return;
//	m_wfx.cbSize = 0;
//
//	// �`�����N��߂�
//	if (mmioAscend(hmmio, &subChunk, 0)) return;
//
//	// data�`�����N�̌���
//	subChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
//	if (mmioDescend(hmmio, &subChunk, &mainChunk, MMIO_FINDCHUNK)) return;
//	m_dataSize = subChunk.cksize;
//
//	// �f�[�^���[�h
//	m_pPCM = new BYTE[m_dataSize];
//	mmioRead(hmmio, (HPSTR)m_pPCM, m_dataSize);
//}

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

// �����ݒ�
void GLWaveManager::Init()
{
	//Resize(GLWAVE_MAX);
}
// �I������
void GLWaveManager::Uninit()
{
	//m_resource.clear();
	ReleaseAll();
}

//==============================================================================

//// ���̕ύX
//void GLWaveManager::Resize(int n)
//{
//	Uninit();
//	m_resource.resize(n);
//}
// ���[�h
void GLWaveManager::Load(LOAD_WAVE* pLoad)
{
	for (auto& pGLWave : *pLoad) {
		if (m_resource.find(pGLWave->m_filename) == m_resource.end()) {
			pGLWave->load();
			m_resource[pGLWave->m_filename] = pGLWave;
		}
	}
}
// ���
void GLWaveManager::ReleaseAll()
{
	//for (auto& res : m_resource) res.release();
	for (auto& item : m_resource) {
		item.second->unload();
	}
	m_resource.clear();
}

//==============================================================================

//// ���\�[�X��Ԃ�
//GLWave* GLWaveManager::getResource(int i)
//{
//	return ((UINT)i < m_resource.size()) ? &m_resource[i] : nullptr;
//}

//******************************************************************************
