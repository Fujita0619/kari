//******************************************************************************
//
//
//		���C�u�����C���^�[�t�F�C�X
//
//
//******************************************************************************
//------< include >-------------------------------------------------------------

#include "GL.h"
using namespace GL;

#include "GLAll.h"

//******************************************************************************
//
//
//		GL�C���^�[�t�F�C�X
//
//
//******************************************************************************
//==============================================================================
//
//		���C�u���������ݒ聕�I�������AWindows API�Ȃ�
//
//==============================================================================

// ���C�u���������ݒ�
void GL::Init( LPCTSTR lpCaption, int width, int height, bool isFullscreen )
{
	HWND hWnd = GLWindow::Init(lpCaption, width, height, isFullscreen);
	if (isFullscreen) GLWindow::CursorOff();	// �t���X�N���[�����J�[�\��OFF

	GLDraw::Init(hWnd, width, height, isFullscreen);
	GLInput::Init(hWnd);
	GLSound::Init();
}

// ���C�u�����I������
void GL::End()
{
	GLSound::Uninit();
	GLInput::Uninit();
	GLDraw::Uninit();
	GLWindow::Uninit();
}

// �Q�[�����[�v����
bool GL::GameLoop() {
	GLSound::Update();
	return GLWindow::MsgLoop();
}

// �J�[�\��ON/OFF
void GL::CursorOn() { GLWindow::CursorOn(); }
void GL::CursorOff() { GLWindow::CursorOff(); }

//==============================================================================
//
//		���͏���
//
//==============================================================================

// ���̓f�o�C�X�̐ݒ�
void GL::SetDevice(DEVICE_ASSIGN*** pppDevice) { GLInput::SetDevice(pppDevice); }

// ���͏��̍X�V
void GL::Input()
{
	GLInput::Update();
	GLWindow::UpdateCursorPos();
}

// ���̓��[�N�̃A�h���X�擾
PAD_WORK *GL::GetPadWork(int i) { return GLInput::GetPadWork(i); }

// �J�[�\�����W�̎擾
int GL::GetCursorPosX() { return (int)GLWindow::GetCursorPos().x; }
int GL::GetCursorPosY() { return (int)GLWindow::GetCursorPos().y; }

//==============================================================================
//
//		�`�搧��
//
//==============================================================================

void GL::BeginScene() { GLDraw::BegineScene(); }
void GL::ClearScene( int argb ) { GLDraw::ClearScene(argb); }
void GL::EndScene() { GLDraw::EndScene(); }

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

// �e�N�X�`��
void GL::LoadTexture(LOAD_TEXTURE* pLoad) { GLDraw::LoadTexture(pLoad); }
void GL::ReleaseTexture() { GLDraw::ReleaseTexture(); }

// �t�H���g
void GL::ResizeFont(int n) { GLDraw::ResizeFont(n); }
void GL::LoadFont(LOAD_FONT* pLoad) { GLDraw::LoadFont(pLoad); }
void GL::ReleaseFont() { GLDraw::ReleaseFont(); }

// ���b�V��
void GL::ResizeMesh(int n) { GLDraw::ResizeMesh(n); }
void GL::LoadMesh(LOAD_MESH* pLoad) { GLDraw::LoadMesh(pLoad); }
void GL::ReleaseMesh() { GLDraw::ReleaseMesh(); }

//==============================================================================
//
//		�`��R�}���h�̒ǉ�
//
//==============================================================================

// �X�v���C�g�`��
void GL::DrawSprite(float x, float y, SPRITE_DATA* data, float sx, float sy, float rot, bool xflip, bool yflip, float sh, float sv, int db, int alpha, int tb, int r, int g, int b, int zcode)
{
	GLDraw::DrawSprite(x, y, data, sx, sy, rot, xflip, yflip, sh, sv, db, tb, D3DCOLOR_ARGB(alpha, r, g, b), zcode);
}

// ������`��
void GL::DrawString(int x, int y, STRING_DATA* data, int db, int alpha, int count, int zcode)
{
	GLDraw::DrawString(x, y, data->fontNum, data->str, data->color, data->isCenter, db, alpha, count, zcode);
}
void GL::DrawString(int x, int y, int fontNum, const char* str, int color, bool isCenter, int db, int alpha,int count, int zcode)
{
	GLDraw::DrawString(x, y, fontNum, str, color, isCenter, db, alpha, count, zcode);
}

// �����`��
void GL::DrawLine(float x0, float y0, float x1, float y1, int color, int db, int alpha, int zcode)
{
	GLDraw::DrawLine(x0, y0, x1, y1, color, db, alpha, zcode);
}

// �l�p�`�`��
void GL::DrawRectangle(float left, float top, float width, float height, int color, int db, int alpha, int zcode)
{
	GLDraw::DrawRectangle(left, top, width, height, color, db, alpha, zcode);
}

// ���b�V���`��
void GL::DrawMesh(MESH_DATA* data, D3DXMATRIX& matTrans, int db, float alpha, float r, float g, float b, int zcode)
{
	GLDraw::DrawMesh(data, matTrans, db, alpha, r, g, b, zcode);
}

//==============================================================================
//
//		�`��ݒ�̒ǉ�
//
//==============================================================================

void GL::SetCamera(CAMERA_DATA* data, int zcode) { GLDraw::SetCamera(data, zcode); }	// �J����
void GL::SetLight(LIGHT_DATA* data, int zcode) { GLDraw::SetLight(data, zcode); }		// ���C�g
void GL::SetAmbient( int rgb, int zcode ) { GLDraw::SetAmbient(rgb, zcode); }			// �A���r�G���g
void GL::SetFog(FOG_DATA* data, int zcode) { GLDraw::SetFog(data, zcode); }				// �t�H�O

//==============================================================================
//
//		���̑�
//
//==============================================================================

// �r���[�}�g���N�X�̎擾
void GL::GetViewMatrix(D3DXMATRIX *pMatrix) { GLDraw::GetViewMatrix(pMatrix); }

// ���[���h���W���X�N���[�����W�ϊ�
void GL::WorldToScreen(float wx, float wy, float wz, int *sx, int *sy) { GLDraw::WorldToScreen(wx, wy, wz, sx, sy); }

//==============================================================================
//
//		�T�E���h
//
//==============================================================================

void GL::Sound() { GLSound::Update(); }
// �T�E���h����
void GL::StopSound() { GLSound::StopSound(); }
void GL::PauseSound() { GLSound::PauseSound(); }
void GL::ResumeSound() { GLSound::ResumeSound(); }

// Wave�t�@�C��
//void GL::ResizeWave(int n) { GLSound::ResizeWave(n); }
void GL::LoadWave(LOAD_WAVE* pLoad) { GLSound::LoadWave(pLoad); }
void GL::ReleaseWave() { GLSound::ReleaseWave(); }

// SE
void GL::ResizeSE(int n) { GLSound::ResizeSE(n); }
void GL::PlaySE(WAVE_DATA& wave) { GLSound::PlaySE(wave); }
void GL::StopSE() { GLSound::StopSE(); }

// BGM
void GL::PlayBGM(WAVE_DATA& wave, bool loop) { GLSound::PlayBGM(wave, loop); }
void GL::StopBGM() { GLSound::StopBGM(); }
void GL::FadeoutBGM(int fadeTime) { GLSound::FadeoutBGM(fadeTime); }
void GL::TempoupBGM() { GLSound::TempoupBGM(); }

// ����
void GL::PlayVoice(WAVE_DATA& wave) { GLSound::PlayVoice(wave); }
void GL::StopVoice() { GLSound::StopVoice(); }

//******************************************************************************
