//******************************************************************************
//
//
//		������`��
//
//
//******************************************************************************

#include "GLAll.h"

#define GLFONT_MAX			3			// �t�H���g���i�f�t�H���g�l�j

//==============================================================================
//
//		���\�[�X
//
//==============================================================================

// �R���X�g���N�^
GLFont::GLFont() : pFont(0)
{
}

//==============================================================================

// �f�X�g���N�^
GLFont::~GLFont()
{
	release();			// �O�̂���
}

//==============================================================================

// �t�H���g�̃��[�h
void GLFont::load(LPDIRECT3DDEVICE9 pd3dDevice, const char* fontname, int size, bool bold, bool italic)
{
	release();
	D3DXCreateFontA(pd3dDevice, -size, 0, (bold) ? FW_BOLD : FW_NORMAL, 1, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontname, &pFont);
}

//==============================================================================

// �t�H���g�̉��
void GLFont::release()
{
	RELEASE(pFont);
}

//==============================================================================

// �f�o�C�X���X�g���̏���
void GLFont::OnLostDevice()
{
	if (pFont) pFont->OnLostDevice();
}

//==============================================================================

// ���Z�b�g���̏���
void GLFont::OnResetDevice()
{
	if (pFont) pFont->OnResetDevice();
}

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

// �����ݒ�
void GLFontManager::Init()
{
	Resize(GLFONT_MAX);
}
// �I������
void GLFontManager::Uninit()
{
	m_resource.clear();
}

//==============================================================================

// ���̕ύX
void GLFontManager::Resize(int n) {
	Uninit();				// �O�̂���
	m_resource.resize(n);
}
// ���[�h
void GLFontManager::Load(LPDIRECT3DDEVICE9 pd3dDevice, LOAD_FONT* pLoad)
{
	while ((UINT)pLoad->fontNum < m_resource.size()) {
		m_resource[pLoad->fontNum].load(pd3dDevice, pLoad->fontname, pLoad->size, pLoad->bold, pLoad->italic);
		pLoad++;
	}
}
// ���
void GLFontManager::ReleaseAll()
{
	for (auto& res : m_resource) res.release();
}

//==============================================================================

// �f�o�C�X���X�g���̏���
void GLFontManager::OnLostDevice()
{
	for (auto& res : m_resource) res.OnLostDevice();
}
// �f�o�C�X���Z�b�g���̏���
void GLFontManager::OnResetDevice()
{
	for (auto& res : m_resource) res.OnResetDevice();
}

//==============================================================================

// ���\�[�X��Ԃ�
LPD3DXFONT GLFontManager::getResource(int i)
{
	return ((UINT)i < m_resource.size()) ? m_resource[i].pFont : nullptr;
}

//==============================================================================
//
//		�`��R�}���h
//
//==============================================================================

// ������
GLDrawFont::GLDrawFont(int x, int y, LPD3DXFONT pFont, const char* str, int argb, bool isCenter, int count) : m_pFont(pFont), m_str(str), m_argb(argb), m_count(count)
{
	// �`����W�ݒ�
	m_rc = { x, y, 0, 0 };
	if (isCenter) {
		RECT	size = { 0, 0, 0, 0 };
		m_pFont->DrawTextA(NULL, m_str, -1, &size, DT_CALCRECT, 0);	// ������T�C�Y�̎擾
		m_rc.left -= size.right / 2;
	}
}

void GLDrawFont::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if (m_pFont) m_pFont->DrawTextA(NULL, m_str, m_count, &m_rc, DT_LEFT | DT_NOCLIP, m_argb);
}

//******************************************************************************
