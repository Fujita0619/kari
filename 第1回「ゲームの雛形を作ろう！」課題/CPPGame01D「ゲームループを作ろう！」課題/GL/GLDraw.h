#ifndef _GLDRAW_H_
#define	_GLDRAW_H_
//******************************************************************************
//
//
//		�`��֘A����
//
//
//******************************************************************************

#include <list>
using namespace std;

//==============================================================================
//
//		���ʃ��x���A�\���́A�C���^�[�t�F�C�X��`
//
//==============================================================================

//--------------------------------
// ���_�f�[�^
//--------------------------------

// ���W�ϊ����Ȃ��A���C�e�B���O���Ȃ�
struct D3DVERTEX_2DCOLOR
{
	float	x, y, z, rhw;		// ���W�ϊ����Ȃ��i�X�N���[�����W�Ŏw��j
	DWORD	diffuse;			// ���_�F
};
#define D3DFVF_2DCOLOR		( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )

// 2D�e�N�X�`���̒��_�f�[�^
struct D3DVERTEX_2DTEXTURE {
	float	x, y, z, rhw;		// ���W�ϊ����Ȃ��i�X�N���[�����W�Ŏw��j
	DWORD	argb;				// ���_�F
	float	tu1, tv1;			// �e�N�X�`��UV���W
};
#define D3DFVF_2DTEXTURE 	( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

// ���W�ϊ�����A���C�e�B���O���Ȃ�
struct D3DVERTEX_3DCOLOR
{
	float	x, y, z;			// ���W�ϊ�����
	DWORD	diffuse;			// ���_�F
};
#define D3DFVF_3DCOLOR		( D3DFVF_XYZ | D3DFVF_DIFFUSE )

// ���W�ϊ�����A���C�e�B���O����
struct D3DVERTEX_3DNORMAL
{
	D3DXVECTOR3		position;		// ���W
	D3DXVECTOR3		normal;			// �@���x�N�g��
};
#define D3DFVF_3DNORMAL		( D3DFVF_XYZ | D3DFVF_NORMAL )


//--------------------------------
// �`��֘A�C���^�[�t�F�[�X
//--------------------------------

// �`��R�}���h
class IGLDrawCmd
{
public:
	virtual ~IGLDrawCmd() {}
	virtual void execute(LPDIRECT3DDEVICE9 pd3dDevice) = 0;
};

//==============================================================================
//
//		�`��Ǘ��N���X
//
//==============================================================================

#define ZCODE_MAX			256					// Z�R�[�h�i���s���j�ő吔

namespace GLDraw
{
	//--------------------------------
	// �����ݒ聕�I������
	//--------------------------------
	HRESULT Init(HWND hWnd, int width, int height, bool isFullscreen);
	void Uninit();

	//--------------------------------
	// �`�搧��
	//--------------------------------
	void BegineScene();
	void ClearScene(int argb);
	void EndScene();

	//--------------------------------
	// ���\�[�X�Ǘ�
	//--------------------------------
	// �e�N�X�`���Ǘ�
	void LoadTexture(LOAD_TEXTURE* pLoad);
	void ReleaseTexture();

	// �t�H���g�Ǘ�
	void ResizeFont(int n);
	void LoadFont(LOAD_FONT* pLoad);
	void ReleaseFont();
	// ���b�V���Ǘ�
	void ResizeMesh(int n);
	void LoadMesh(LOAD_MESH* pLoad);
	void ReleaseMesh();

	//--------------------------------
	// �`��R�}���h�̒ǉ�
	//--------------------------------
	void DrawSprite(float x, float y, SPRITE_DATA* data, float sx = 1.0f, float sy = 1.0f, float rot = 0.0f, bool xflip = false, bool yflip = false, float sh = 1.0f, float sv = 1.0f, int db = DB_COPY, int tb = TB_TEX, int argb = 0xffffffff, int zcode = 0);
	void DrawString(int x, int y, int fontNum, const char* str, int color = COLOR_WHITE, bool isCenter = false, int db = DB_COPY, int alpha = 255, int count = -1, int zcode = 0);
	void DrawLine(float x0, float y0, float x1, float y1, int color = COLOR_WHITE, int db = DB_COPY, int alpha = 255, int zcode = 0);
	void DrawRectangle(float left, float top, float width, float height, int color = COLOR_WHITE, int db = DB_COPY, int alpha = 255, int zcode = 0);
	void DrawMesh(MESH_DATA* data, D3DXMATRIX& matTrans, int db = DB_COPY, float alpha = 1.0f, float r = 0.0f, float g = 0.0f, float b = 0.0f, int zcode = 255);

	//--------------------------------
	// �`��ݒ�̒ǉ�
	//--------------------------------
	void SetCamera(CAMERA_DATA* data, int zcode = 255);
	void SetLight(LIGHT_DATA* data, int zcode = 255);
	void SetAmbient(int rgb, int zcode = 255);
	void SetFog(FOG_DATA* data, int zcode = 255);

	//--------------------------------
	// ���̑�
	//--------------------------------
	// �����_�����O�X�e�[�g�̐ݒ�
	void GetViewMatrix(D3DXMATRIX* pMatrix);
	void WorldToScreen(float wx, float wy, float wz, int *sx, int *sy);
};

//==============================================================================
//
//		�`��R�}���h
//
//==============================================================================

//--------------------------------
// �e�N�X�`���u�����_�ݒ�
//--------------------------------
class GLDrawSetTB : public IGLDrawCmd
{
private:
	int		m_tb;
public:
	GLDrawSetTB(int tb);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//--------------------------------
// �f�B�X�e�B�l�[�V�����u�����_�ݒ�
//--------------------------------
class GLDrawSetDB : public IGLDrawCmd
{
private:
	int		m_db;
public:
	GLDrawSetDB(int db);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//--------------------------------
// �����`��
//--------------------------------
class GLDrawLine : public IGLDrawCmd
{
private:
	D3DVERTEX_2DCOLOR v[2];
public:
	GLDrawLine(float x0, float y0, float x1, float y1, int argb);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//--------------------------------
// �l�p�`�`��
//--------------------------------
class GLDrawRectangle : public IGLDrawCmd
{
private:
	D3DVERTEX_2DCOLOR v[4];
public:
	GLDrawRectangle(float left, float top, float width, float height, int argb = COLOR_WHITE);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//--------------------------------
// �����X�e�[�g�ݒ�
//--------------------------------
class GLDrawSetInitState : public IGLDrawCmd
{
public:
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//--------------------------------
// �J�����ݒ�
//--------------------------------
class GLDrawSetCamera : public IGLDrawCmd
{
private:
	D3DXMATRIX		matView;
	D3DXMATRIX		matProj;
public:
	GLDrawSetCamera(CAMERA_DATA* data);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//--------------------------------
// ���C�g�ݒ�
//--------------------------------
class GLDrawSetLight : public IGLDrawCmd
{
private:
	LIGHT_DATA*		m_data;
public:
	GLDrawSetLight(LIGHT_DATA* data);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//--------------------------------
// �A���r�G���g�ݒ�
//--------------------------------
class GLDrawSetAmbient : public IGLDrawCmd
{
private:
	int				m_rgb;
public:
	GLDrawSetAmbient(int rgb);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//--------------------------------
// �t�H�O�ݒ�
//--------------------------------
class GLDrawSetFog : public IGLDrawCmd
{
private:
	FOG_DATA*		m_data;
public:
	GLDrawSetFog(FOG_DATA* data);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//******************************************************************************
#endif // !_GLDRAW_H_
