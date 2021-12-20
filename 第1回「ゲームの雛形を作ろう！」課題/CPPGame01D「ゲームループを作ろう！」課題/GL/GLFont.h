#ifndef _STRING_H_
#define	_STRING_H_
//******************************************************************************
//
//
//		������`��
//
//
//******************************************************************************

#include <vector>
using namespace std;

//==============================================================================
//
//		���\�[�X
//
//==============================================================================

struct GLFont
{
	LPD3DXFONT	pFont;

	GLFont();
	~GLFont();

	void load(LPDIRECT3DDEVICE9 pd3dDevice9, const char *fontname, int size, bool bold = false, bool italic = false);
	void release();

	void OnLostDevice();
	void OnResetDevice();
};

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

// �t�H���g�Ǘ��N���X
class GLFontManager
{
private:
	vector<GLFont>	m_resource;
public:
	static GLFontManager* getInstance()
	{
		static GLFontManager instance;
		return &instance;
	}

	void Init();
	void Uninit();

	void Resize(int n);
	void Load(LPDIRECT3DDEVICE9 pd3dDevice, LOAD_FONT* pLoad);
	void ReleaseAll();

	void OnLostDevice();
	void OnResetDevice();

	LPD3DXFONT getResource(int i);
};

#define pGLFontManager		(GLFontManager::getInstance())

//==============================================================================
//
//		�`��R�}���h
//
//==============================================================================

// ������
class GLDrawFont : public IGLDrawCmd
{
private:
	LPD3DXFONT	m_pFont;
	const char*		m_str;
	int			m_argb;
	int			m_count;
	RECT		m_rc;
public:
	GLDrawFont(int x, int y, LPD3DXFONT pFont, const char* str, int argb = COLOR_WHITE, bool isCenter = false, int count = -1);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//******************************************************************************
#endif // !_STRING_H_