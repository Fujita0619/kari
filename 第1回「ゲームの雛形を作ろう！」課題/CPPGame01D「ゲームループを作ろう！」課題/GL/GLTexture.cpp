//******************************************************************************
//
//
//		�e�N�X�`��
//
//
//******************************************************************************

#include "GLAll.h"

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

// �����ݒ�
void GLTextureManager::Init()
{
}

// �I������
void GLTextureManager::Uninit()
{
	ReleaseAll();
}

//==============================================================================

// ���[�h
void GLTextureManager::Load(LPDIRECT3DDEVICE9 pd3dDevice, LOAD_TEXTURE* pLoad)
{
	for (auto& pGLTexture : *pLoad) {
		if (m_resource.find(pGLTexture->m_filename) == m_resource.end()) {
			pGLTexture->load(pd3dDevice);
			m_resource[pGLTexture->m_filename] = pGLTexture;
		}
	}
}

// ���
void GLTextureManager::ReleaseAll()
{
	for (auto& item : m_resource) {
		item.second->unload();
	}
	m_resource.clear();
}

//==============================================================================
//
//		�`��R�}���h
//
//==============================================================================

// �e�N�X�`���`��
GLDrawTexure::GLDrawTexure(float x, float y, SPRITE_DATA* data, float sx, float sy, float rot, bool xflip, bool yflip, float sh, float sv, int argb) : m_pd3dTexture(0)
{
	TEXTURE_DATA*	pTexture = data->m_pTexture;
	if (!pTexture) return;

	m_pd3dTexture = pTexture->m_pd3dTexture;
	if (!m_pd3dTexture) return;

	float	left = data->m_left;
	float	top  = data->m_top;
	float	width = data->m_width;
	float	height = data->m_height;

	// �X�v���C�g�p���_�̐ݒ�i���ʕ����j
	m_v[0].z    = m_v[1].z    = m_v[2].z    = m_v[3].z    = 0;
	m_v[0].rhw  = m_v[1].rhw  = m_v[2].rhw  = m_v[3].rhw  = 1;
	m_v[0].argb = m_v[1].argb = m_v[2].argb = m_v[3].argb = argb;

	// UV���W�i�e�N�X�`���̈�j�̐ݒ�
	float	u0 = left / pTexture->m_width;
	float	u1 = (left + width) / pTexture->m_width;

	if (data->m_flip & SPR_XFLIP) xflip = !xflip;
	if (xflip) {
		float	tmp = u0;
		u0 = u1;
		u1 = tmp;
	}

	float	v0 = top / pTexture->m_height;
	float	v1 = (top + height) / pTexture->m_height;

	if (data->m_flip & SPR_YFLIP) yflip = !yflip;
	if (yflip) {
		float	tmp = v0;
		v0 = v1;
		v1 = tmp;
	}

	m_v[0].tu1 = u0;	m_v[0].tv1 = v0;
	m_v[1].tu1 = u1;	m_v[1].tv1 = v0;
	m_v[2].tu1 = u0;	m_v[2].tv1 = v1;
	m_v[3].tu1 = u1;	m_v[3].tv1 = v1;

	// ���_���W�i�X�N���[�����W�j�̐ݒ�
	D3DXMATRIX	matTrans;
	D3DXMATRIX	matDummy;				// �v�Z�p�_�~�[
	D3DXMatrixIdentity(&matTrans);
	matTrans *= *D3DXMatrixTranslation(&matDummy, data->m_xofs, data->m_yofs, 0);		// ���s�ړ��i���[�J�����_�̐ݒ�j
	matTrans *= *D3DXMatrixScaling(&matDummy, sx, sy, 1.0);		// �g��k���i�摜XY�����j
	matTrans *= *D3DXMatrixRotationZ(&matDummy, rot);			// ��]
	matTrans *= *D3DXMatrixScaling(&matDummy, sh, sv, 1.0);		// �g��k���i�X�N���[�������j
	matTrans *= *D3DXMatrixTranslation(&matDummy, x, y, 0);		// ���s�ړ��i��ʍ��W�ݒ�j

	m_v[0].x = matTrans._41;
	m_v[0].y = matTrans._42;

	m_v[1].x = width *matTrans._11 + matTrans._41;
	m_v[1].y = width *matTrans._12 + matTrans._42;

	m_v[2].x = height*matTrans._21 + matTrans._41;
	m_v[2].y = height*matTrans._22 + matTrans._42;

	m_v[3].x = width *matTrans._11 + height*matTrans._21 + matTrans._41;
	m_v[3].y = width *matTrans._12 + height*matTrans._22 + matTrans._42;

	// 0.5�h�b�g�␳
	float hx0 = matTrans._11;
	float hy0 = matTrans._12;
	float d0 = sqrtf(hx0*hx0 + hy0*hy0);
	hx0 /= d0;
	hy0 /= d0;

	float hx1 = matTrans._21;
	float hy1 = matTrans._22;
	float d1 = sqrtf(hx1*hx1 + hy1*hy1);
	hx1 /= d1;
	hy1 /= d1;

	float hx = (hx0 + hx1) / 2.0f;
	float hy = (hy0 + hy1) / 2.0f;

	m_v[0].x += hx;	m_v[0].y += hy;
	m_v[1].x -= hx;	m_v[1].y += hy;
	m_v[2].x += hx;	m_v[2].y -= hy;
	m_v[3].x -= hx;	m_v[3].y -= hy;
}

void GLDrawTexure::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if (!m_pd3dTexture) return;

	pd3dDevice->SetTexture(0, m_pd3dTexture);
	pd3dDevice->SetFVF(D3DFVF_2DTEXTURE);
	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_v, sizeof(D3DVERTEX_2DTEXTURE));
}

//******************************************************************************
