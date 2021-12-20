#ifndef _GLWINDOW_H_
#define	_GLWINDOW_H_
//******************************************************************************
//
//
//		Windows�֘A����
//
//
//******************************************************************************

#include <Windows.h>

namespace GLWindow
{
	HWND Init(LPCTSTR lpCaption, int width, int height, bool isFullscreen = true);			// �E�C���h�E�����ݒ�
	void Uninit();				// �E�C���h�E�I������
	bool MsgLoop();				// ���b�Z�[�W����

	void CursorOn();			// �J�[�\���\��
	void CursorOff();			// �J�[�\������
	void UpdateCursorPos();		// �J�[�\�����W�̍X�V
	POINT GetCursorPos();		// �J�[�\�����W�i�N���C�A���g���W�j��Ԃ�
};

//******************************************************************************
#endif // !_GLWINDOW_H_