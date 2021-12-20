//******************************************************************************
//
//
//		Windows�֘A����
//
//
//******************************************************************************

// �C���N���[�h
#include "GLAll.h"
#include <tchar.h>

static HWND		m_hWnd;
static POINT	m_cursorPos;

static LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	// ���b�Z�[�W����

//==============================================================================
//
//		�E�C���h�E�����ݒ�ƏI������
//
//==============================================================================

// �E�C���h�E�̏����ݒ�i�o�^�ƍ쐬�j
HWND GLWindow::Init( LPCTSTR lpCaption, int width, int height, bool isFullscreen )
{
	//	�E�C���h�E�N���X�̓o�^
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), LoadIcon( NULL, IDI_APPLICATION ), LoadCursor( NULL, IDC_ARROW ), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL,
		_T("My Window"), LoadIcon( NULL, IDI_APPLICATION ) };

	RegisterClassEx( &wc );			// �E�C���h�E�N���X��o�^


	// �E�C���h�E�̍쐬
	m_hWnd = CreateWindow( _T("My Window"), lpCaption,
		( isFullscreen )? WS_POPUP: ( WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX ),
		0, 0, 640, 480,
		NULL, NULL, GetModuleHandle(NULL), NULL );

	// �E�C���h�E�̈ʒu�ƃT�C�Y�̒���
	RECT	rcWindow;
	RECT	rcClient;
	GetWindowRect(m_hWnd, &rcWindow);				// �E�C���h�E�S�̂̃T�C�Y�擾
	GetClientRect(m_hWnd, &rcClient);				// �N���C�A���g�̈�̃T�C�Y�擾

	int w = width  + ( rcWindow.right  - rcWindow.left ) - ( rcClient.right  - rcClient.left );
	int h = height + ( rcWindow.bottom - rcWindow.top  ) - ( rcClient.bottom - rcClient.top  );

	SetWindowPos(m_hWnd, 0, 0, 0, w, h, SWP_NOZORDER);


	ShowWindow(m_hWnd, SW_SHOWDEFAULT);				// �E�C���h�E�̊O�g�̕\��
	UpdateWindow(m_hWnd);							// �N���C�A���g�̈�i�����j�̍X�V

	return m_hWnd;
}

//==============================================================================

// �E�C���h�E�̏I������
void GLWindow::Uninit()
{
	UnregisterClass( _T("My Window"), GetModuleHandle(NULL) );
}

//==============================================================================
//
//		���b�Z�[�W����
//
//==============================================================================

// ���b�Z�[�W���[�v
bool GLWindow::MsgLoop()
{
	MSG		msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
		if (msg.message == WM_QUIT) return false;		// �A�v���P�[�V�����I��
		TranslateMessage(&msg);			// ���z�L�[�R�[�h��ASCII�R�[�h�ɕϊ�
		DispatchMessage(&msg);			// WndProc�փ��b�Z�[�W�z�M
	}
	return true;						// ���b�Z�[�W���[�v�I���i�Q�[�������ցj
}

//==============================================================================

// �E�C���h�E�v���V�[�W��
static LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// ���b�Z�[�W�̎�ނɉ����ď����𕪊�
	switch ( msg ) {
	case WM_KEYDOWN:	// �L�[�������ꂽ�Ƃ�
		switch( wParam ) {
		case VK_ESCAPE:		// ESC�L�[�̂Ƃ�
			PostMessage( hWnd, WM_CLOSE, 0, 0 );	// WM_CLOSE���s
			return 0;
		}
		break;

	case WM_DESTROY:	// �E�C���h�E���j�����ꂽ�Ƃ��̏���
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//==============================================================================
//
//		���̑�
//
//==============================================================================

// �J�[�\���\��
void GLWindow::CursorOn()
{
	while (ShowCursor(TRUE) < 0) {}
}
// �J�[�\������
void GLWindow::CursorOff()
{
	while (ShowCursor(FALSE) >= 0) {}
}

//==============================================================================

// �J�[�\�����W�̍X�V
void GLWindow::UpdateCursorPos()
{
	GetCursorPos(&m_cursorPos);
	ScreenToClient(m_hWnd, &m_cursorPos);
}
// �J�[�\�����W�i�N���C�A���g���W�j��Ԃ�
POINT GLWindow::GetCursorPos()
{
	return m_cursorPos;
}

//******************************************************************************
