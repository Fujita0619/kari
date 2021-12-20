//******************************************************************************
//
//
//		�e�탉�x���A�f�[�^�^��`
//
//
//******************************************************************************

#include "GLType.h"

//******************************************************************************
//
//
//		�`��֘A
//
//
//******************************************************************************
//==============================================================================
//
//		�e�N�X�`��
//
//==============================================================================

// �J���[�L�[���x��
enum {
	COLORKEY_NONE,			// �����F�Ȃ�
	COLORKEY_BLACK,			// �u���v������
	COLORKEY_LEFTTOP,		// �u�摜�̍���Ɠ����F�v������
};

// �R���X�g���N�^
TEXTURE_DATA::TEXTURE_DATA(const char* filename, int colorKey) : m_pd3dTexture(0), m_filename(filename), m_colorKey(colorKey)
{
}

TEXTURE_NORMAL::TEXTURE_NORMAL(const char* filename) : TEXTURE_DATA(filename, COLORKEY_NONE) {}
TEXTURE_BLACK::TEXTURE_BLACK(const char* filename) : TEXTURE_DATA(filename, COLORKEY_BLACK) {}
TEXTURE_LEFTTOP::TEXTURE_LEFTTOP(const char* filename) : TEXTURE_DATA(filename, COLORKEY_LEFTTOP) {}

//==============================================================================

// �f�X�g���N�^
TEXTURE_DATA::~TEXTURE_DATA()
{
	unload();
}

//==============================================================================

// �e�N�X�`���̃��[�h
void TEXTURE_DATA::load(LPDIRECT3DDEVICE9 pd3dDevice)
{
	unload();

	// �摜�̃��[�h�ƃe���|�����e�N�X�`���̍쐬
	LPDIRECT3DTEXTURE9	pTmpTexture;		// �e���|�����e�N�X�`��
	if (FAILED(D3DXCreateTextureFromFileExA(pd3dDevice, m_filename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, NULL, NULL, &pTmpTexture))) return;

	// �J���[�L�[�̐ݒ�
	IDirect3DSurface9	*pTmpSurface;		// �e���|�����e�N�X�`���̃T�[�t�F�C�X
	D3DLOCKED_RECT		LockedRect;			// �T�[�t�F�C�X���b�N�̈�
	D3DCOLOR			ColorKey;			// �J���[�L�[

	pTmpTexture->GetSurfaceLevel(0, &pTmpSurface);		// �e���|�����e�N�X�`���̃T�[�t�F�C�X�擾

	switch (m_colorKey) {
	case COLORKEY_BLACK:		// ���𓧖��ɂ���ꍇ
		ColorKey = D3DCOLOR_ARGB(255, 0, 0, 0);			// �u�s�����ȍ��v
		break;
	case COLORKEY_LEFTTOP:		// �摜����Ɠ����F�𓧖��ɂ���ꍇ
		pTmpSurface->LockRect(&LockedRect, NULL, D3DLOCK_READONLY);
		ColorKey = *((D3DCOLOR *)LockedRect.pBits);			// �u�摜�̍���̐F�v�擾
		pTmpSurface->UnlockRect();
		break;
	default:					// ����ȊO
		ColorKey = D3DCOLOR_ARGB(0, 0, 0, 0);;			// �u�����ȍ��v
		break;
	}

	// �{���̃e�N�X�`���쐬
	D3DSURFACE_DESC		desc;				// �e���|�����e�N�X�`���̃T�[�t�F�C�X���
	pTmpSurface->GetDesc(&desc);			// �T�[�t�F�C�X���擾
	m_width = (float)desc.Width;
	m_height = (float)desc.Height;

	D3DXCreateTexture(pd3dDevice, desc.Width, desc.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pd3dTexture);

	// �摜�]���i�e���|�������{���j
	IDirect3DSurface9	*pSurface;
	m_pd3dTexture->GetSurfaceLevel(0, &pSurface);		// �T�[�t�F�C�X�擾
	D3DXLoadSurfaceFromSurface(pSurface, NULL, NULL, pTmpSurface, NULL, NULL, D3DX_FILTER_NONE, ColorKey);	// �]��
	pSurface->Release();

	// �e���|�����̍폜
	pTmpSurface->Release();							// �e���|�����e�N�X�`���̃T�[�t�F�C�X���
	pTmpTexture->Release();							// �e���|�����e�N�X�`���̉��
}

//==============================================================================

// �e�N�X�`���̉��
void TEXTURE_DATA::unload()
{
	if (m_pd3dTexture) {
		m_pd3dTexture->Release();
		m_pd3dTexture = nullptr;
	}
}

//==============================================================================
//
//		�X�v���C�g�f�[�^
//
//==============================================================================

// �R���X�g���N�^
SPRITE_DATA::SPRITE_DATA(TEXTURE_DATA* pTexture, float left, float top, float width, float height, float xofs, float yofs, int flip)
	: m_pTexture(pTexture), m_left(left), m_top(top), m_width(width), m_height(height), m_xofs(xofs), m_yofs(yofs), m_flip(flip)
{
}

SPRITE_LEFTTOP::SPRITE_LEFTTOP(TEXTURE_DATA* pTexture, float left, float top, float width, float height, int flip) : SPRITE_DATA(pTexture, left, top, width, height, 0, 0, flip) {}
SPRITE_CENTER::SPRITE_CENTER(TEXTURE_DATA* pTexture, float left, float top, float width, float height, int flip) : SPRITE_DATA(pTexture, left, top, width, height, -width / 2, -height / 2, flip) {}
SPRITE_BOTTOM::SPRITE_BOTTOM(TEXTURE_DATA* pTexture, float left, float top, float width, float height, int flip) : SPRITE_DATA(pTexture, left, top, width, height, -width / 2, -height, flip) {}

//==============================================================================
//
//		������f�[�^
//
//==============================================================================

// ������f�[�^
STRING_DATA::STRING_DATA(int fontNum, char* str, int color, bool isCenter)
{
	this->fontNum = fontNum;
	this->str = str;
	this->color = color;
	this->isCenter = isCenter;
}

// ������f�[�^�i���l�߁j
STRING_LEFT::STRING_LEFT(int fontNum, char* str, int color) : STRING_DATA(fontNum, str, color, false)
{
}
STRING_LEFT_S::STRING_LEFT_S(char* str, int color) : STRING_LEFT(FONT_S, str, color) {}
STRING_LEFT_M::STRING_LEFT_M(char* str, int color) : STRING_LEFT(FONT_M, str, color) {}
STRING_LEFT_L::STRING_LEFT_L(char* str, int color) : STRING_LEFT(FONT_L, str, color) {}

// ������f�[�^�i�Z���^�����O�j
STRING_CENTER::STRING_CENTER(int fontNum, char* str, int color) : STRING_DATA(fontNum, str, color, true)
{
}
STRING_CENTER_S::STRING_CENTER_S(char* str, int color) : STRING_CENTER(FONT_S, str, color) {}
STRING_CENTER_M::STRING_CENTER_M(char* str, int color) : STRING_CENTER(FONT_M, str, color) {}
STRING_CENTER_L::STRING_CENTER_L(char* str, int color) : STRING_CENTER(FONT_L, str, color) {}

//==============================================================================
//
//		���b�V���f�[�^
//
//==============================================================================

// �R���X�g���N�^�iY����]�ƃX�P�[���̎w��j
MESH_DATA::MESH_DATA(int meshNum, float scale, float rot)
{
	this->meshNum = meshNum;
	SetMatrix(D3DXVECTOR3(scale, scale, scale), D3DXVECTOR3(0.0f, rot, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

// �R���X�g���N�^�i�t���w��j
MESH_DATA::MESH_DATA(int meshNum, D3DXVECTOR3& scale, D3DXVECTOR3& rot, D3DXVECTOR3& trans)
{
	this->meshNum = meshNum;
	SetMatrix(scale, rot, trans);
}

// �f�[�^�␳�s��̐ݒ�
void MESH_DATA::SetMatrix(const D3DXVECTOR3& scale, const D3DXVECTOR3& rot, const D3DXVECTOR3& trans)
{
	D3DXMATRIX	matTmp;
	D3DXMatrixIdentity(&matData);
	matData *= *D3DXMatrixScaling(&matTmp, scale.x, scale.y, scale.z);
	matData *= *D3DXMatrixRotationYawPitchRoll(&matTmp, rot.y, rot.x, rot.z);
	matData._41 = trans.x;
	matData._42 = trans.y;
	matData._43 = trans.z;
}

//==============================================================================
//
//		Wave�f�[�^
//
//==============================================================================

WAVE_DATA::WAVE_DATA(char * filename) : m_filename(filename), m_pPCM(0)
{
}

WAVE_DATA::~WAVE_DATA()
{
	unload();
}

void WAVE_DATA::load()
{
	unload();

	HMMIO hmmio = mmioOpenA(m_filename, nullptr, MMIO_ALLOCBUF | MMIO_READ);
	if (hmmio) {
		loadPCM(hmmio);
		mmioClose(hmmio, 0);
	}
}

void WAVE_DATA::unload()
{
	if (m_pPCM) {
		delete[] m_pPCM;
		m_pPCM = nullptr;
	}
}

// PCM�f�[�^�̃��[�h
void WAVE_DATA::loadPCM(HMMIO hmmio)
{
	MMCKINFO		mainChunk;
	MMCKINFO		subChunk;

	// RIFF�`�����N��WAVE�^�C�v����
	mainChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if (mmioDescend(hmmio, &mainChunk, NULL, MMIO_FINDRIFF)) return;

	// fmt�`�����N�̌���
	subChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hmmio, &subChunk, &mainChunk, MMIO_FINDCHUNK)) return;

	// �t�H�[�}�b�g�̓ǂݍ���
	if (mmioRead(hmmio, (HPSTR)&m_wfx, sizeof(m_wfx)) != sizeof(m_wfx)) return;
	if (m_wfx.wFormatTag != WAVE_FORMAT_PCM) return;
	m_wfx.cbSize = 0;

	// �`�����N��߂�
	if (mmioAscend(hmmio, &subChunk, 0)) return;

	// data�`�����N�̌���
	subChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hmmio, &subChunk, &mainChunk, MMIO_FINDCHUNK)) return;
	m_dataSize = subChunk.cksize;

	// �f�[�^���[�h
	m_pPCM = new BYTE[m_dataSize];
	mmioRead(hmmio, (HPSTR)m_pPCM, m_dataSize);
}

//******************************************************************************
