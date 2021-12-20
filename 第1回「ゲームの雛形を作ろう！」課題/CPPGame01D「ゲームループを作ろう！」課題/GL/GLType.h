#ifndef _GLTYPE_H_
#define _GLTYPE_H_
//******************************************************************************
//
//
//		�e�탉�x���A�f�[�^�^��`
//
//
//******************************************************************************

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
using namespace std;

//******************************************************************************
//
//
//		���͊֘A
//
//
//******************************************************************************

// ���̓f�o�C�X�ԍ�
enum GLDEVICE
{
	GLKEYBOARD,			// �L�[�{�[�h�i�}�E�X���j
	GLJOYSTICK,			// �W���C�X�e�B�b�N
};

// PadState�r�b�g���蓖��
#define PAD_UP			0x00000001
#define PAD_DOWN		0x00000002
#define PAD_LEFT		0x00000004
#define PAD_RIGHT		0x00000008
#define PAD_START		0x00000010
#define PAD_SELECT		0x00000020
#define PAD_TRG1		0x00000040
#define PAD_TRG2		0x00000080
#define PAD_TRG3		0x00000100
#define PAD_TRG4		0x00000200
#define PAD_L1			0x00000400
#define PAD_R1			0x00000800
#define PAD_L2			0x00001000
#define PAD_R2			0x00002000
#define PAD_L3			0x00004000
#define PAD_R3			0x00008000

// �W���C�X�e�B�b�N�p�{�^�����x��
enum {
	JOY_BUTTON_01,
	JOY_BUTTON_02,
	JOY_BUTTON_03,
	JOY_BUTTON_04,
	JOY_BUTTON_05,
	JOY_BUTTON_06,
	JOY_BUTTON_07,
	JOY_BUTTON_08,
	JOY_BUTTON_09,
	JOY_BUTTON_10,
	JOY_BUTTON_11,
	JOY_BUTTON_12,
	JOY_BUTTON_13,
	JOY_BUTTON_14,
	JOY_BUTTON_15,
	JOY_BUTTON_16,
};

// �r�b�g���蓖�ăf�[�^
struct PAD_ASSIGN
{
	int			bit;				// pad_state�p�r�b�g�f�[�^
	int			code;				// �Ή�����L�[�i�{�^���j�R�[�h
};

// ���̓f�o�C�X���蓖��
#pragma warning( disable : 4200 )
struct DEVICE_ASSIGN
{
	GLDEVICE	device;				// �g�p����f�o�C�X
	PAD_ASSIGN	assign[];			// �r�b�g���蓖�ăf�[�^
};
#pragma warning( default : 4200 )

// ���̓f�[�^
struct PAD_WORK
{
	int			state;				// �ʏ����
	int			trg;				// �g���K�[����
	float		volume[4];			// �A�i���O����
};

//******************************************************************************
//
//
//		�`��֘A
//
//
//******************************************************************************
//==============================================================================
//
//		�`��ݒ�
//
//==============================================================================

// �J���[���x���i0x00RRGGBB�j
#define COLOR_BLACK		0x00000000			// ��
#define COLOR_GRAY		0x00808080			// �D�F
#define COLOR_WHITE		0x00ffffff			// ��

#define COLOR_RED		0x00ff0000			// ��
#define COLOR_GREEN		0x0000ff00			// ��
#define COLOR_BLUE		0x000000ff			// ��

#define COLOR_CYAN		0x0000ffff			// ���F
#define COLOR_MAGENTA	0x00ff00ff			// ��
#define COLOR_YELLOW	0xffffff00			// ���F

// �e�N�X�`���u�����_�ݒ�
enum {
	TB_TEX,			// �e�N�X�`���J���[�ŕ\��
	TB_COLOR,		// ���_�J���[�ŕ\��
	TB_ADD,			// �e�N�X�`���J���[�{���_�J���[�i�u���C�g���Z�j
	TB_SUB,			// �e�N�X�`���J���[�|���_�J���[�i�u���C�g���Z�j
	TB_MUL,			// �e�N�X�`���J���[�~���_�J���[�i�R���g���X�g�j
};

// �f�B�X�e�B�l�[�V�����u�����_�ݒ�
enum {
	DB_COPY,		// �ʏ�i�����F����j
	DB_MIX,			// �������i�A���t�@�u�����h�j
	DB_ADD,			// �J���[���Z
	DB_SUB,			// �J���[���Z
};

//==============================================================================
//
//		�e�N�X�`���֘A
//
//==============================================================================

// �e�N�X�`���f�[�^
struct TEXTURE_DATA
{
	LPDIRECT3DTEXTURE9	m_pd3dTexture;
	const char*			m_filename;			// �t�@�C����
	int					m_colorKey;			// �J���[�L�[
	float				m_width;			// ����
	float				m_height;			// ����

	TEXTURE_DATA(const char* filename, int colorKey);
	~TEXTURE_DATA();
	void load(LPDIRECT3DDEVICE9 pd3dDevice);
	void unload();
};

// �����F�ݒ�Ȃ��i�e�N�X�`���̃A���t�@�����̂܂܎g�p�j
struct TEXTURE_NORMAL : public TEXTURE_DATA
{
	TEXTURE_NORMAL(const char* filename);
};

// �u���v�𓧖��F�ɂ���
struct TEXTURE_BLACK : public TEXTURE_DATA
{
	TEXTURE_BLACK(const char* filename);
};

// �u�e�N�X�`���̍���̐F�Ɠ����F�v�𓧖��F�ɂ���
struct TEXTURE_LEFTTOP : public TEXTURE_DATA
{
	TEXTURE_LEFTTOP(const char* filename);
};

//==============================================================================

// ���[�h�f�[�^
typedef vector<TEXTURE_DATA*>	LOAD_TEXTURE;

//==============================================================================

// �摜���]�t���O
#define SPR_XFLIP		0x01
#define	SPR_YFLIP		0x02

// �X�v���C�g�f�[�^
struct SPRITE_DATA
{
	TEXTURE_DATA*	m_pTexture;
	float			m_left, m_top;
	float			m_width, m_height;
	float			m_xofs, m_yofs;
	int				m_flip;

	SPRITE_DATA(TEXTURE_DATA* pTexture, float left, float top, float width, float height, float xofs = 0.0f, float yofs = 0.0f, int flip = 0x00);
};

// �X�v���C�g�f�[�^�i��_�F����j
struct SPRITE_LEFTTOP : public SPRITE_DATA
{
	SPRITE_LEFTTOP(TEXTURE_DATA* pTexture, float left, float top, float width, float height, int flip = 0x00);
};

// �X�v���C�g�f�[�^�i��_�F���S�j
struct SPRITE_CENTER : public SPRITE_DATA
{
	SPRITE_CENTER(TEXTURE_DATA* pTexture, float left, float top, float width, float height, int flip = 0x00);
};

// �X�v���C�g�f�[�^�i��_�F�����j
struct SPRITE_BOTTOM : public SPRITE_DATA
{
	SPRITE_BOTTOM(TEXTURE_DATA* pTexture, float left, float top, float width, float height, int flip = 0x00);
};

//==============================================================================
//
//		������
//
//==============================================================================

// �t�H���g�����f�[�^
struct LOAD_FONT
{
	int			fontNum;		// �t�H���g�ԍ�
	const char*	fontname;		// �t�H���g��
	int			size;			// �T�C�Y�i�����j
	bool		bold;			// ����
	bool		italic;			// �Α�
};

#define FONT_S		0
#define FONT_M		1
#define FONT_L		2

// ������f�[�^
struct STRING_DATA
{
	int		fontNum;
	char*	str;
	int		color;
	bool	isCenter;

	STRING_DATA(int fontNum, char* str, int color = COLOR_WHITE, bool isCenter = false);
};

// ������f�[�^�i���l�߁j
struct STRING_LEFT : public STRING_DATA
{
	STRING_LEFT(int fontNum, char* str, int color = COLOR_WHITE);
};
struct STRING_LEFT_S : public STRING_LEFT
{
	STRING_LEFT_S(char* str, int color = COLOR_WHITE);
};
struct STRING_LEFT_M : public STRING_LEFT
{
	STRING_LEFT_M(char* str, int color = COLOR_WHITE);
};
struct STRING_LEFT_L : public STRING_LEFT
{
	STRING_LEFT_L(char* str, int color = COLOR_WHITE);
};

// ������f�[�^�i�Z���^�����O�j
struct STRING_CENTER : public STRING_DATA
{
	STRING_CENTER(int fontNum, char*str, int color = COLOR_WHITE);
};
struct STRING_CENTER_S : public STRING_CENTER
{
	STRING_CENTER_S(char*str, int color = COLOR_WHITE);
};
struct STRING_CENTER_M : public STRING_CENTER
{
	STRING_CENTER_M(char*str, int color = COLOR_WHITE);
};
struct STRING_CENTER_L : public STRING_CENTER
{
	STRING_CENTER_L(char*str, int color = COLOR_WHITE);
};

//==============================================================================
//
//		���b�V��
//
//==============================================================================

// ���b�V�����[�h�f�[�^
struct LOAD_MESH
{
	int		meshNum;		// ���b�V���ԍ�
	char*	filename;		// �t�@�C����
};

// ���b�V���f�[�^
struct MESH_DATA
{
	int				meshNum;			// ���b�V���ԍ�
	D3DXMATRIX		matData;			// �f�[�^�␳�s��

	MESH_DATA(int meshNum, float scale = 1.0f, float rot = 0.0f);
	MESH_DATA(int meshNum, D3DXVECTOR3& scale, D3DXVECTOR3& rot, D3DXVECTOR3& trans);
private:
	// �f�[�^�␳�s��̐ݒ�
	void SetMatrix(const D3DXVECTOR3& scale = D3DXVECTOR3(1, 1, 1), const D3DXVECTOR3& rot = D3DXVECTOR3(0, 0, 0), const D3DXVECTOR3& trans = D3DXVECTOR3(0, 0, 0));
};

//==============================================================================
//
//		�`��ݒ�
//
//==============================================================================

// �J�����f�[�^
struct CAMERA_DATA
{
	// �r���[�}�g���N�X
	D3DXVECTOR3		pos;				// �J�����̍��W
	D3DXVECTOR3		look;				// �����_
	D3DXVECTOR3		up;					// �J�����̉�]�i������x�N�g���j
	// �v���W�F�N�V����
	float			fovy;				// Y��������p
	float			aspect;				// �A�X�y�N�g��i�����������j
	float			znear, zfar;		// �j�A�v���[���A�t�@�[�v���[��
};

// ���C�g�f�[�^
struct LIGHT_DATA
{
	D3DVECTOR		direction;			// ����
	D3DCOLORVALUE	color;				// �F�i�f�B�t���[�Y�A�X�y�L�����[�j
};

// �t�H�O�f�[�^
struct FOG_DATA
{
	D3DCOLOR		color;				// �t�H�O�̐F
	float			zStart;				// �t�H�O�J�n�ʒu
	float			zEnd;				// �t�H�O�I���ʒu
};

//******************************************************************************
//
//
//		�T�E���h�֘A
//
//
//******************************************************************************

//struct LOAD_WAVE
//{
//	int		waveNum;		// Wave�ԍ�
//	char*	filename;		// �t�@�C����
//};

//// �T�E���h�f�[�^
//struct WAVE_DATA
//{
//	char*	m_filename;			// �t�@�C����
//	WAVE_DATA(char* filename);
//	~WAVE_DATA();
//	void load();
//	void unload();
//};

// Wave�t�@�C��
struct WAVE_DATA
{
	char*			m_filename;			// �t�@�C����
	WAVEFORMATEX	m_wfx;				// �t�H�[�}�b�g
	DWORD			m_dataSize;			// �f�[�^�T�C�Y
	BYTE*			m_pPCM;				// PCM�f�[�^

	WAVE_DATA(char* filename);
	~WAVE_DATA();
	void load();
	void unload();
	void loadPCM(HMMIO hmmio);

	WAVEFORMATEX	getWfx() { return m_wfx; }
	DWORD			getDataSize() { return m_dataSize; }
	BYTE*			getPCM() { return m_pPCM; }
};

// ���[�h�f�[�^
typedef vector<WAVE_DATA*>	LOAD_WAVE;

//******************************************************************************
#endif		// #ifndef _GLTYPE_H_
