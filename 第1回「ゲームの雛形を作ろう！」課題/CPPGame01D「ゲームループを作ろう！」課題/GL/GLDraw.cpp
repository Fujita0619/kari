//******************************************************************************
//
//
//		�`��֘A����
//
//
//******************************************************************************

#pragma comment( lib, "d3d9.lib" )
#pragma comment ( lib, "d3dx9.lib" )
#pragma comment ( lib, "winmm.lib" )

//------< include >-------------------------------------------------------------

#include "GLAll.h"

//------< extern >--------------------------------------------------------------
//------< define >--------------------------------------------------------------

// �t���[�����̒�`
#define FPS				60				// �t���[�����[�g
#define CHECK_TIME		(6000/FPS)		// 1�t���[��������̎��ԁi�P��1/6�~���b�F(1000/60)*(60/FPS)*6�j

//------< structure >-----------------------------------------------------------

// �f�o�C�X�ݒ�f�[�^�\����
struct SETTING_DATA
{
	D3DDEVTYPE				DevType;					// �f�o�C�X�^�C�v
	DWORD					BehaviorFlags;				// �f�o�C�X���쐧��t���O
	D3DFORMAT				DisplayFormat;				// �f�B�X�v���C�i�o�b�N�o�b�t�@�j�t�H�[�}�b�g
	UINT					RefreshRate;				// ���t���b�V�����[�g
};

//------< data >----------------------------------------------------------------

// �t���X�N���[�����[�h�̃f�o�C�X�ݒ�f�[�^�i�D�揇�j
static SETTING_DATA setting_data[] = {
	{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING, D3DFMT_X8R8G8B8, FPS },
	{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING, D3DFMT_R5G6B5  , FPS },
	{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING, D3DFMT_X8R8G8B8, D3DPRESENT_RATE_DEFAULT },
	{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING, D3DFMT_R5G6B5  , D3DPRESENT_RATE_DEFAULT },
	{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DFMT_X8R8G8B8, FPS },
	{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DFMT_R5G6B5  , FPS },
	{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DFMT_X8R8G8B8, D3DPRESENT_RATE_DEFAULT },
	{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DFMT_R5G6B5  , D3DPRESENT_RATE_DEFAULT },
	{ D3DDEVTYPE_REF, 0, D3DFMT_UNKNOWN , 0 },	// �I���R�[�h
};

// �f�t�H���g�t�H���g
static LOAD_FONT load_font[] = {
	{ FONT_S, "�l�r �S�V�b�N", 16, true, false },
	{ FONT_M, "�l�r �S�V�b�N", 32, true, false },
	{ FONT_L, "�l�r �S�V�b�N", 64, true, false },
	{ -1, "", 0, 0, 0 }
};

//------< work >----------------------------------------------------------------

static LPDIRECT3D9				m_pD3D;						// Direct3D�C���^�[�t�F�C�X
static LPDIRECT3DDEVICE9		m_pd3dDevice;				// Direct3D�f�o�C�X�C���^�[�t�F�C�X
static D3DPRESENT_PARAMETERS	m_d3dpp;					// �f�o�C�X�̓����ݒ肷��\����
static list<IGLDrawCmd*>		m_drawList[ZCODE_MAX];		// �`�惊�X�g

enum LASTSET
{
	LASTSET_INITSTATE,
	LASTSET_CAMERA,
	LASTSET_LIGHT,
	LASTSET_AMBIENT,
	LASTSET_FOG,
	LASTSET_MAX
};

static IGLDrawCmd*				m_lastSetting[LASTSET_MAX];

//------< prototype >-----------------------------------------------------------

static HRESULT CreateFullScreenDevice(HWND hWnd);
static HRESULT CreateWindowDevice(HWND hWnd);

static void addList(int zcode, IGLDrawCmd* cmd);
static void addSetting(LASTSET i, IGLDrawCmd* cmd);

//==============================================================================
//
//		�����ݒ聕�I������
//
//==============================================================================

// �����ݒ�
HRESULT GLDraw::Init(HWND hWnd, int width, int height, bool isFullscreen)
{
	ZeroMemory(&m_lastSetting, sizeof(m_lastSetting));

	// Direct3D�C���^�[�t�F�C�X�̎擾
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) return E_FAIL;

	// Direct3D�f�o�C�X�C���^�[�t�F�C�X�̎擾
	if (FAILED((isFullscreen) ? CreateFullScreenDevice(hWnd) : CreateWindowDevice(hWnd))) return E_FAIL;

	// ���\�[�X�̐ݒ�
	pGLFontManager->Init();
	pGLTextureManager->Init();
	pGLMeshManager->Init();

	pGLFontManager->Load(m_pd3dDevice, load_font);

	// �`��X�e�[�g�̏����ݒ�
	addList(ZCODE_MAX - 1, new GLDrawSetInitState);
	addSetting(LASTSET_INITSTATE, new GLDrawSetInitState);

	return S_OK;
}

//------------------------------------------------------------------------------

// �t���X�N���[�����[�h�̃f�o�C�X�ݒ�
static HRESULT CreateFullScreenDevice(HWND hWnd)
{
	// �f�o�C�X����̐ݒ�
	RECT			rcClient;
	GetClientRect(hWnd, &rcClient);
	m_d3dpp.Windowed = FALSE;							// �E�C���h�E���[�h�i�t���X�N���[���j
	m_d3dpp.BackBufferWidth = rcClient.right;			// �o�b�N�o�b�t�@�̉���
	m_d3dpp.BackBufferHeight = rcClient.bottom;			// �o�b�N�o�b�t�@�̍���
	m_d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// �X���b�v�G�t�F�N�g

	m_d3dpp.EnableAutoDepthStencil = TRUE;				// Z�o�b�t�@
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// �o�b�t�@�̃t�H�[�}�b�g
	//m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// Z�o�b�t�@�̃t�H�[�}�b�g

	// �Z�b�e�B���O�f�[�^�ɂ���ݒ��Ђ��[���玎��
	SETTING_DATA	*setting = setting_data;
	while (setting->DisplayFormat != D3DFMT_UNKNOWN) {

		m_d3dpp.BackBufferFormat = setting->DisplayFormat;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
		m_d3dpp.FullScreen_RefreshRateInHz = setting->RefreshRate;	// ���t���b�V�����[�g
		m_d3dpp.PresentationInterval = (setting->RefreshRate == FPS) ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

		if (SUCCEEDED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, setting->DevType, hWnd, setting->BehaviorFlags, &m_d3dpp, &m_pd3dDevice))) return S_OK;
		setting++;
	}

	return E_FAIL;
}

//------------------------------------------------------------------------------

// �E�C���h�E���[�h�̃f�o�C�X�ݒ�
static HRESULT CreateWindowDevice(HWND hWnd)
{
	//���݂̃��[�h���擾
	D3DDISPLAYMODE	dmode;
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dmode);

	// �f�o�C�X����̐ݒ�
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.Windowed = TRUE;							// �E�C���h�E���[�h�i�E�C���h�E�j
	m_d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// �X���b�v�G�t�F�N�g
	m_d3dpp.BackBufferFormat = dmode.Format;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g

	m_d3dpp.EnableAutoDepthStencil = TRUE;				// Z�o�b�t�@
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// Z�o�b�t�@�̃t�H�[�}�b�g
	//m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// Z�o�b�t�@�̃t�H�[�}�b�g�i�X�e���V������j

	// �f�o�C�X�̐���
	if (SUCCEEDED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice))) return S_OK;
	return m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice);
}

//==============================================================================

// �I������
void GLDraw::Uninit()
{
	pGLMeshManager->Uninit();		// ���b�V���̉��
	pGLTextureManager->Uninit();	// �e�N�X�`���̉��
	pGLFontManager->Uninit();		// �t�H���g�̉��

	for (auto& item : m_lastSetting) {
		if (item) delete item;
	}

	// Direct3D�̏I������
	RELEASE(m_pd3dDevice);
	RELEASE(m_pD3D);
}

//==============================================================================
//
//		�`�搧��
//
//==============================================================================

// �`��J�n
void GLDraw::BegineScene()
{
	m_pd3dDevice->BeginScene();
}

//==============================================================================

// ��ʃN���A
void GLDraw::ClearScene(int argb)
{
	//m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, argb, 1.0f, 0 );
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, argb, 1.0f, 0);
}

//==============================================================================

// �`��I�����\��
void GLDraw::EndScene()
{
	// �`�惊�X�g�ɂ��`��
	for (int i = ZCODE_MAX - 1; i >= 0; i--) {
		for (auto& cmd : m_drawList[i]) {
			cmd->execute(m_pd3dDevice);
			delete cmd;
		}
		m_drawList[i].clear();
	}

	// �`��I��
	m_pd3dDevice->EndScene();

	// �V�[���̕\��
	switch (m_d3dpp.PresentationInterval) {
	// Vsync�����̂Ƃ�
	case D3DPRESENT_INTERVAL_ONE:
	case D3DPRESENT_INTERVAL_TWO:
	case D3DPRESENT_INTERVAL_THREE:
	case D3DPRESENT_INTERVAL_FOUR:
		break;
	// �^�C�}�[�Ńt���[���X�V�̂Ƃ�
	default:
		static DWORD updateTime = timeGetTime() * 6;
		while (timeGetTime() * 6 - updateTime < CHECK_TIME) Sleep(1);		// �t���[�����ԁi16.66*6 = 100�j���z����܂ő҂�
		updateTime += CHECK_TIME;
		break;
	}

	// �o�b�N�o�b�t�@�̕\��
	if (m_pd3dDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST) {
		// �f�o�C�X���X�g�̂Ƃ�
		if (m_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			pGLFontManager->OnLostDevice();
			if (SUCCEEDED(m_pd3dDevice->Reset(&m_d3dpp))) {

				// �`��X�e�[�g�̕���
				for (auto& item : m_lastSetting) {
					if (item) item->execute(m_pd3dDevice);
				}

				pGLFontManager->OnResetDevice();
			}
		}
	}
}

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

// �e�N�X�`��
void GLDraw::LoadTexture(LOAD_TEXTURE* pLoad) { pGLTextureManager->Load(m_pd3dDevice, pLoad); }
void GLDraw::ReleaseTexture() { pGLTextureManager->ReleaseAll(); }

// �t�H���g
void GLDraw::ResizeFont(int n) { pGLFontManager->Resize(n); }
void GLDraw::LoadFont(LOAD_FONT* pLoad) { pGLFontManager->Load(m_pd3dDevice, pLoad); }
void GLDraw::ReleaseFont() { pGLFontManager->ReleaseAll(); }

// ���b�V��
void GLDraw::ResizeMesh(int n) { pGLMeshManager->Resize(n); }
void GLDraw::LoadMesh(LOAD_MESH* pLoad) { pGLMeshManager->Load(m_pd3dDevice, pLoad); }
void GLDraw::ReleaseMesh() { pGLMeshManager->ReleaseAll(); }

//==============================================================================
//
//		�R�}���h�ǉ�
//
//==============================================================================

// �`�惊�X�g�ǉ�
static void addList(int zcode, IGLDrawCmd* cmd)
{
	if ((UINT)zcode < ZCODE_MAX) m_drawList[zcode].push_back(cmd);
}
// �Z�b�e�B���O�ǉ�
static void addSetting(LASTSET i, IGLDrawCmd* cmd)
{
	if (m_lastSetting[i]) delete m_lastSetting[i];
	m_lastSetting[i] = cmd;
}

//==============================================================================
//
//		�`��R�}���h�̒ǉ�
//
//==============================================================================

//-------------------------------
// �X�v���C�g�`��
//-------------------------------
void GLDraw::DrawSprite(float x, float y, SPRITE_DATA* data, float sx, float sy, float rot, bool xflip, bool yflip, float sh, float sv, int db, int tb, int argb, int zcode)
{
	if (!data) return;

	addList(zcode, new GLDrawSetDB(db));
	addList(zcode, new GLDrawSetTB(tb));
	addList(zcode, new GLDrawTexure(x, y, data, sx, sy, rot, xflip, yflip, sh, sv, argb));
}

//-------------------------------
// ������`��
//-------------------------------

void GLDraw::DrawString(int x, int y, int fontNum, const char* str, int color, bool isCenter, int db, int alpha, int count, int zcode)
{
	if (!str) return;
	if (count == 0) return;

	LPD3DXFONT	pFont = pGLFontManager->getResource(fontNum);
	if (!pFont) return;

	addList(zcode, new GLDrawSetDB(db));
	addList(zcode, new GLDrawFont(x, y, pFont, str, (alpha << 24) | (color & 0x00ffffff), isCenter, count));
}

//-------------------------------
// �����`��
//-------------------------------
void GLDraw::DrawLine(float x0, float y0, float x1, float y1, int color, int db, int alpha, int zcode)
{
	addList(zcode, new GLDrawSetDB(db));
	addList(zcode, new GLDrawLine(x0, y0, x1, y1, (alpha << 24) | (color & 0x00ffffff)));
}

//-------------------------------
// �l�p�`�`��
//-------------------------------
void GLDraw::DrawRectangle(float left, float top, float width, float height, int color, int db, int alpha, int zcode)
{
	addList(zcode, new GLDrawSetDB(db));
	addList(zcode, new GLDrawRectangle(left, top, width, height, (alpha << 24) | (color & 0x00ffffff)));
}

//-------------------------------
// ���b�V���`��
//-------------------------------
void GLDraw::DrawMesh(MESH_DATA* data, D3DXMATRIX& matTrans, int db, float alpha, float r, float g, float b, int zcode)
{
	if (!data) return;
	addList(zcode, new GLDrawSetDB(db));
	addList(zcode, new GLDrawMesh(data, matTrans, alpha, r, g, b));
}

//==============================================================================
//
//		�`��ݒ�̒ǉ�
//
//==============================================================================

//-------------------------------
// �J�����ݒ�
//-------------------------------
void GLDraw::SetCamera(CAMERA_DATA* data, int zcode)
{
	if (!data) return;
	addList(zcode, new GLDrawSetCamera(data));
	addSetting(LASTSET_CAMERA, new GLDrawSetCamera(data));
}

//-------------------------------
// ���C�g�ݒ�
//-------------------------------
void GLDraw::SetLight(LIGHT_DATA* data, int zcode)
{
	addList(zcode, new GLDrawSetLight(data));
	addSetting(LASTSET_LIGHT, new GLDrawSetLight(data));
}

//-------------------------------
// �A���r�G���g�ݒ�
//-------------------------------
void GLDraw::SetAmbient(int rgb, int zcode)
{
	addList(zcode, new GLDrawSetAmbient(rgb));
	addSetting(LASTSET_AMBIENT, new GLDrawSetAmbient(rgb));
}

//-------------------------------
// �t�H�O�ݒ�
//-------------------------------
void GLDraw::SetFog(FOG_DATA* data, int zcode)
{
	addList(zcode, new GLDrawSetFog(data));
	addSetting(LASTSET_FOG, new GLDrawSetFog(data));
}

//==============================================================================
//
//		�`��R�}���h
//
//==============================================================================

// �e�N�X�`���u�����_
GLDrawSetTB::GLDrawSetTB(int tb) : m_tb(tb)
{
}

void GLDrawSetTB::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	// �J���[�̐ݒ�
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	// ARG1���e�N�X�`���J���[
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);	// ARG2�����_�J���[

	switch (m_tb) {
	default:
	case TB_TEX:
		// �J���[���e�N�X�`���J���[
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		break;
	case TB_COLOR:
		//case D3D_TBNOTEX:
		// �J���[�����_�J���[
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
		break;
	case TB_ADD:
		// �J���[���e�N�X�`���J���[�{���_�J���[
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
		break;
	case TB_SUB:
		// �J���[���e�N�X�`���J���[�|���_�J���[
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		break;
	case TB_MUL:
		// �J���[���e�N�X�`���J���[�~���_�J���[
		pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		break;
	}

	// �A���t�@�̐ݒ�
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

//==============================================================================

// �f�B�X�e�B�l�[�V�����u�����_
GLDrawSetDB::GLDrawSetDB(int db) : m_db(db)
{
}

void GLDrawSetDB::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	switch (m_db) {
	default:
	case DB_COPY:
		// �A���t�@���e�N�X�`���A���t�@�i���_�A���t�@�͖����j
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		// �A���t�@�u�����h�����Ȃ̂Őݒ肵�Ȃ�
		//pd3dDevice->SetRenderState(D3DRS_SRCBLEND , D3DBLEND_ONE           );
		//pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO          );
		//pd3dDevice->SetRenderState(D3DRS_BLENDOP  , D3DBLENDOP_ADD         );
		break;
	case DB_MIX:
		// �J���[��SRC�J���[�~SRC�A���t�@�{DST�J���[�~�i�P�|SRC�A���t�@�j
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		break;
	case DB_ADD:
		// �J���[��SRC�J���[�~SRC�A���t�@�{DST�J���[
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		break;
	case DB_SUB:
		// �J���[��-SRC�J���[�~SRC�A���t�@�{DST�J���[
		pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		break;
	}

	pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, (m_db == DB_COPY) ? TRUE : FALSE);
}

//==============================================================================

// �����`��
GLDrawLine::GLDrawLine(float x0, float y0, float x1, float y1, int argb)
{
	v[0].x = x0;
	v[0].y = y0;

	v[1].x = x1;
	v[1].y = y1;

	v[0].z       = v[1].z       = 0.0f;
	v[0].rhw     = v[1].rhw     = 1.0f;
	v[0].diffuse = v[1].diffuse = argb;
}

void GLDrawLine::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	pd3dDevice->SetTexture(0, NULL);
	pd3dDevice->SetFVF(D3DFVF_2DCOLOR);
	pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, v, sizeof(D3DVERTEX_2DCOLOR));
}

//==============================================================================

// �l�p�`�`��
GLDrawRectangle::GLDrawRectangle(float left, float top, float width, float height, int argb)
{
	v[0].x = left;
	v[0].y = top;

	v[1].x = left + width;
	v[1].y = top;

	v[2].x = left;
	v[2].y = top + height;

	v[3].x = left + width;
	v[3].y = top + height;

	v[0].z       = v[1].z       = v[2].z       = v[3].z       = 0.0f;
	v[0].rhw     = v[1].rhw     = v[2].rhw     = v[3].rhw     = 1.0f;
	v[0].diffuse = v[1].diffuse = v[2].diffuse = v[3].diffuse = argb;
}

void GLDrawRectangle::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	pd3dDevice->SetTexture(0, NULL);
	pd3dDevice->SetFVF(D3DFVF_2DCOLOR);
	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(D3DVERTEX_2DCOLOR));
}

//==============================================================================

// �����X�e�[�g�ݒ�
void GLDrawSetInitState::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// �A���t�@�e�X�g�F�L��
	pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);					// ��A���t�@�l�F0x00
	pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		// �A���t�@��r�F��A���t�@�l���傫���Ƃ�OK
}

//==============================================================================

// �J�����ݒ�
GLDrawSetCamera::GLDrawSetCamera(CAMERA_DATA* data)
{
	D3DXMatrixLookAtLH(&matView, &data->pos, &data->look, &data->up);
	D3DXMatrixPerspectiveFovLH(&matProj, data->fovy, data->aspect, data->znear, data->zfar);
}
void GLDrawSetCamera::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

//==============================================================================

// ���C�g�ݒ�
GLDrawSetLight::GLDrawSetLight(LIGHT_DATA* data) : m_data(data)
{
}
void GLDrawSetLight::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if (m_data) {
		D3DLIGHT9		light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Direction = m_data->direction;
		light.Diffuse = light.Specular = m_data->color;
		pd3dDevice->SetLight(0, &light);
		pd3dDevice->LightEnable(0, TRUE);
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else {
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
}

//==============================================================================

// �A���r�G���g�ݒ�
GLDrawSetAmbient::GLDrawSetAmbient(int rgb) : m_rgb(rgb)
{
}
void GLDrawSetAmbient::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	pd3dDevice->SetRenderState(D3DRS_AMBIENT, m_rgb);
}

//==============================================================================

// �t�H�O�ݒ�
GLDrawSetFog::GLDrawSetFog(FOG_DATA* data) : m_data(data)
{
}
void GLDrawSetFog::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if (m_data) {
		// ���`��ԃt�H�O
		pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		// �t�H�O�̏��ݒ�
		pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, m_data->color);
		pd3dDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&m_data->zStart)));
		pd3dDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&m_data->zEnd)));
		// �t�H�O����
		pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}
	else {
		pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
}

//==============================================================================
//
//		���̑�
//
//==============================================================================

// �r���[�}�g���N�X�̎擾
void GLDraw::GetViewMatrix(D3DXMATRIX *pMatrix)
{
	m_pd3dDevice->GetTransform(D3DTS_VIEW, pMatrix);
}

//==============================================================================

// �X�N���[�����W�ւ̕ϊ�
void GLDraw::WorldToScreen(float wx, float wy, float wz, int *sx, int *sy)
{
	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9	viewport;
	m_pd3dDevice->GetViewport(&viewport);

	// �ϊ��s��̎擾
	D3DXMATRIX	matProj, matView, matWorld;
	m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matWorld);

	// �X�N���[�����W�̎擾
	D3DXVECTOR3		screenPos;
	D3DXVECTOR3		worldPos(wx, wy, wz);
	D3DXVec3Project(&screenPos, &worldPos, &viewport, &matProj, &matView, &matWorld);
	*sx = (int)screenPos.x;
	*sy = (int)screenPos.y;
}

//******************************************************************************