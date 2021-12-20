#ifndef _GL_H_
#define	_GL_H_
//******************************************************************************
//
//
//		���C�u�����C���^�[�t�F�C�X
//
//
//******************************************************************************

#include <windows.h>
#include <tchar.h>

#include "GLType.h"		// �e�탉�x���A�f�[�^�^��`

namespace GL
{
//-------------------------------
// ���ʃ��x��
//-------------------------------

#define	PI		3.1415926535898f

//==============================================================================
//
//		���C�u���������ݒ聕�I�������AWindows API�Ȃ�
//
//==============================================================================

// ���C�u���������ݒ�
void Init( LPCTSTR lpCaption, int width=640, int height=480, bool isFullscreen=true );

// ���C�u�����I������
void End();

// �Q�[�����[�v���s�`�F�b�N�i���Ԃ̓��b�Z�[�W���[�v�j
bool GameLoop();

// �J�[�\���\��
void CursorOn();

// �J�[�\������
void CursorOff();

//==============================================================================
//
//		���͏���
//
//==============================================================================

// ���̓f�o�C�X�̐ݒ�
void SetDevice(DEVICE_ASSIGN*** pppDevice);

// ���͏��̍X�V
void Input();

// ���̓��[�N�̃A�h���X�擾
PAD_WORK* GetPadWork(int i = 0);

#define pad_w			(GetPadWork(0))

#define pad_state		(pad_w->state)
#define pad_trg			(pad_w->trg)
#define pad_volumeX		(pad_w->volume[0])
#define pad_volumeY		(pad_w->volume[1])
#define pad_volumeZ		(pad_w->volume[2])
#define pad_volumeRZ	(pad_w->volume[3])

#define pad_state1		(pad_w[0].state)
#define pad_trg1		(pad_w[0].trg)

#define pad_state2		(pad_w[1].state)
#define pad_trg2		(pad_w[1].trg)

#define pad_state3		(pad_w[2].state)
#define pad_trg3		(pad_w[2].trg)

#define pad_state4		(pad_w[3].state)
#define pad_trg4		(pad_w[3].trg)

// �J�[�\�����W�̎擾
int GetCursorPosX();
int GetCursorPosY();

#define mouseX			(GetCursorPosX())
#define mouseY			(GetCursorPosY())

//==============================================================================
//
//		�`�揈��
//
//==============================================================================

//--------------------------------
// �V�[������
//--------------------------------

// �`��J�n
void BeginScene();

// �V�[���N���A
void ClearScene(int argb = COLOR_BLACK);

// �`��I�����\��
void EndScene();

//--------------------------------
// ���\�[�X�Ǘ�
//--------------------------------

// �e�N�X�`��
void LoadTexture(LOAD_TEXTURE* pLoad);		// ���[�h
void ReleaseTexture();						// ���

// �t�H���g
void ResizeFont(int n);						// ���̕ύX
void LoadFont(LOAD_FONT* pLoad);			// ���[�h
void ReleaseFont();							// ���

// ���b�V��
void ResizeMesh(int n);						// ���̕ύX
void LoadMesh(LOAD_MESH* pLoad);			// ���[�h
void ReleaseMesh();							// ���

//--------------------------------
// �`��
//--------------------------------

// �X�v���C�g�`��
void DrawSprite(
	float x, float y,									// �X�N���[�����W
	SPRITE_DATA* data,									// �X�v���C�g�f�[�^
	float sx = 1.0f, float sy = 1.0f,					// �g�嗦�i�摜�����j
	float rot = 0.0f,									// ��]�p�x�i���W�A���j
	bool xflip = false, bool yflip = false,				// �摜���]�t���O
	float sh = 1.0f, float sv = 1.0f,					// �g�嗦�i��ʕ����j
	int db = DB_COPY, int alpha = 255,					// �f�B�X�e�B�l�[�V�����u�����_�ݒ�
	int tb = TB_TEX, int r = 0, int g = 0, int b = 0,	// �e�N�X�`���u�����_�ݒ�
	int zcode = 0										// Z�R�[�h�i�`��D�揇�ʁj
);

// ������`��
void DrawString(
	int x, int y,										// �X�N���[�����W
	int fontNum,										// �t�H���g�ԍ�
	const char* str,											// ������
	int color = COLOR_WHITE,							// �J���[
	bool isCenter = false,								// �Z���^�����O�t���O�ifalse�F���l�߁Atrue�F�Z���^�����O�j
	int db = DB_COPY, int alpha = 255,					// �f�B�X�e�B�l�[�V�����u�����_�ݒ�
	int count = -1,										// �`�敶�����i-1�F�����񂷂ׂāj
	int zcode = 0										// Z�R�[�h�i�`��D�揇�ʁj
);
// ������`��iS�T�C�Y�AM�T�C�Y�AL�T�C�Y�j
inline void DrawStringS(int x, int y, const char* str, int color = COLOR_WHITE, bool isCenter = false, int db = DB_COPY, int alpha = 255, int count = -1, int zcode = 0)
{
	DrawString(x, y, FONT_S, str, color, isCenter, db, alpha, count, zcode);
}
inline void DrawStringM(int x, int y, const char* str, int color = COLOR_WHITE, bool isCenter = false, int db = DB_COPY, int alpha = 255, int count = -1, int zcode = 0)
{
	DrawString(x, y, FONT_M, str, color, isCenter, db, alpha, count, zcode);
}
inline void DrawStringL(int x, int y, const char* str, int color = COLOR_WHITE, bool isCenter = false, int db = DB_COPY, int alpha = 255, int count = -1, int zcode = 0)
{
	DrawString(x, y, FONT_L, str, color, isCenter, db, alpha, count, zcode);
}

// ������`��iSTRING_DATA�g�p�j
void DrawString(
	int x, int y,										// �X�N���[�����W
	STRING_DATA* data,									// ������f�[�^
	int db = DB_COPY, int alpha = 255,					// �f�B�X�e�B�l�[�V�����u�����_�ݒ�
	int count = -1,										// �`�敶�����i-1�F�����񂷂ׂāj
	int zcode = 0										// Z�R�[�h�i�`��D�揇�ʁj
);

// �����`��
void DrawLine(
	float x0, float y0,									// �n�_
	float x1, float y1,									// �I�_
	int color = COLOR_WHITE,							// �J���[
	int db = DB_COPY, int alpha = 255,					// �f�B�X�e�B�l�[�V�����u�����_�ݒ�
	int zcode = 0										// Z�R�[�h�i�`��D�揇�ʁj
);

// �l�p�`�`��
void DrawRectangle(
	float left, float top,								// ������W
	float width, float height,							// �����A����
	int color = COLOR_WHITE,							// �J���[
	int db = DB_COPY, int alpha = 255,					// �f�B�X�e�B�l�[�V�����u�����_�ݒ�
	int zcode = 0										// Z�R�[�h�i�`��D�揇�ʁj
);

// ���b�V���̕`��
void DrawMesh(
	MESH_DATA *data,									// ���b�V���f�[�^
	D3DXMATRIX& matTrans,								// ���[���h�ϊ��s��
	int db = DB_COPY, float alpha = 1.0f,				// �f�B�X�e�B�l�[�V�����u�����_�ݒ�
	float r = 0.0f, float g = 0.0f, float b = 0.0f,		// �J���[���Z�l
	int zcode = 255										// Z�R�[�h�i�`��D�揇�ʁj
);

//--------------------------------
// �`��ݒ�
//--------------------------------

// �J�����̐ݒ�
void SetCamera(CAMERA_DATA* data, int zcode = 255);

// ���C�g�̐ݒ�
void SetLight(LIGHT_DATA* data, int zcode = 255);

// �A���r�G���g�̐ݒ�
void SetAmbient( int rgb, int zcode = 255 );

// �t�H�O�̐ݒ�
void SetFog(FOG_DATA *data, int zcode = 255);

//--------------------------------
// ���̑�
//--------------------------------

// �r���[�}�g���N�X�̎擾
void GetViewMatrix(D3DXMATRIX *pMatrix);

// ���[���h���W���X�N���[�����W�ϊ�
void WorldToScreen(float wx, float wy, float wz, int *sx, int *sy);

//==============================================================================
//
//		�T�E���h
//
//==============================================================================

void Sound();

// �S�T�E���h����
void StopSound();
void PauseSound();
void ResumeSound();

// Wave
//void ResizeWave(int n);						// ���̕ύX
void LoadWave(LOAD_WAVE* pLoad);			// ���[�h
void ReleaseWave();							// ���

// SE
void ResizeSE(int n);						// �����Đ����̕ύX
void PlaySE(WAVE_DATA& wave);
void StopSE();

// BGM
void PlayBGM(WAVE_DATA& wave, bool loop = true);
void StopBGM();
void FadeoutBGM(int fadeTime = 120);	// �t�F�[�h�A�E�g�i2�b�j
void TempoupBGM();						// BGM�̃e���|�A�b�v�i1.1�{�j

// ����
void PlayVoice(WAVE_DATA& wave);
void StopVoice();
};

using namespace GL;

//******************************************************************************
#endif // !_GL_H_