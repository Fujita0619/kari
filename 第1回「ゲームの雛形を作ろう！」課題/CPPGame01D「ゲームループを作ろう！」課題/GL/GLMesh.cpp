//******************************************************************************
//
//
//		���b�V��
//
//
//******************************************************************************

#include "GLAll.h"

#define GLMESH_MAX			100			// ���b�V�����i�f�t�H���g�l�j

//==============================================================================
//
//		���\�[�X
//
//==============================================================================

// �R���X�g���N�^
GLMesh::GLMesh() : pMesh(0)
{
}

//==============================================================================

// �f�X�g���N�^
GLMesh::~GLMesh()
{
	release();
}

//==============================================================================

// ���b�V���̃��[�h
void GLMesh::load(LPDIRECT3DDEVICE9 pd3dDevice, char *filename)
{
	release();

	// ���b�V���̃��[�h
	LPD3DXBUFFER	pD3DXMtrlBuffer;
	if (FAILED(D3DXLoadMeshFromXA(filename, D3DXMESH_SYSTEMMEM, pd3dDevice, NULL, &pD3DXMtrlBuffer, NULL, &dwNumMaterials, &pMesh))) {
		release();
		return;
	}

	// X�t�@�C���̃p�X�擾
	char	strPath[MAX_PATH];
	ZeroMemory(strPath, sizeof(strPath));
	strncpy_s(strPath, MAX_PATH, filename, _TRUNCATE);
	for (int i = MAX_PATH - 1; i >= 0; i--) {
		if (strPath[i] == '/') break;
		strPath[i] = '\0';
	}

	// �}�e���A���ƃe�N�X�`���̐ݒ�
	D3DXMATERIAL*	d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMaterials = new D3DMATERIAL9[dwNumMaterials];
	pTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++) {
		// �}�e���A���̐ݒ�
		pMaterials[i] = d3dxMaterials[i].MatD3D;				// �}�e���A���̃R�s�[
		pMaterials[i].Ambient = pMaterials[i].Diffuse;			// �A���r�G���g�ɒ��_�F���R�s�[

		// �e�N�X�`���̐ݒ�
		pTextures[i] = nullptr;
		if ((d3dxMaterials[i].pTextureFilename) && (strlen(d3dxMaterials[i].pTextureFilename) > 0)) {
			char	strTexture[MAX_PATH];
			strncpy_s(strTexture, MAX_PATH, strPath, _TRUNCATE);
			strcat_s(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
			//pTextures[i] = D3D_LoadTexture(strTexture, colorKey);
			HRESULT hr = D3DXCreateTextureFromFileA(pd3dDevice, strTexture, &pTextures[i]);
			if (FAILED(hr)) {
				//MessageBox( "�e�N�X�`����������܂���I", strTexture );
				pTextures[i] = nullptr;
			}
		}
	}

	// �o�b�t�@�̉��
	pD3DXMtrlBuffer->Release();
}

//==============================================================================

// ���b�V���̉��
void GLMesh::release()
{
	if (!pMesh) return;

	// �}�e���A���z��̉��
	if (pMaterials) delete[] pMaterials;

	// �X�̃e�N�X�`���ƃe�N�X�`���z��̉��
	if (pTextures) {
		for (DWORD i = 0; i < dwNumMaterials; i++) {
			RELEASE(pTextures[i]);
		}
		delete[] pTextures;
	}

	// ���b�V���i�W�I���g���j�̉��
	if (pMesh) pMesh->Release();
	pMesh = nullptr;
}

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

// �����ݒ�
void GLMeshManager::Init()
{
	Resize(GLMESH_MAX);
}
// �I������
void GLMeshManager::Uninit()
{
	m_resource.clear();
}

//==============================================================================

// ���̕ύX
void GLMeshManager::Resize(int n)
{
	Uninit();				// �O�̂���
	m_resource.resize(n);
}
// ���[�h
void GLMeshManager::Load(LPDIRECT3DDEVICE9 pd3dDevice, LOAD_MESH* pLoad)
{
	while ((UINT)pLoad->meshNum < m_resource.size()) {
		m_resource[pLoad->meshNum].load(pd3dDevice, pLoad->filename);
		pLoad++;
	}
}
// ���b�V������i���ׂāj
void GLMeshManager::ReleaseAll()
{
	for (auto& res : m_resource) res.release();
}

//==============================================================================

// ���\�[�X��Ԃ�
GLMesh* GLMeshManager::getResource(int i)
{
	return ((UINT)i < m_resource.size()) ? &m_resource[i] : nullptr;
}

//==============================================================================
//
//		�`��R�}���h
//
//==============================================================================

// �͈̓`�F�b�N
static float rangeCheck(float color)
{
	if (color < 0.0f) color = 0.0f;
	if (color > 1.0f) color = 1.0f;
	return color;
}

//==============================================================================

// ���b�V���`��
GLDrawMesh::GLDrawMesh(MESH_DATA* pMesh, D3DXMATRIX& matTrans, float alpha, float r, float g, float b)
{
	m_matTrans = pMesh->matData * matTrans;
	m_pMesh = pGLMeshManager->getResource(pMesh->meshNum);
	m_alpha = alpha;
	m_r = r;
	m_g = g;
	m_b = b;
}

void GLDrawMesh::execute(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if (!m_pMesh) return;

	pd3dDevice->SetTransform(D3DTS_WORLD, &m_matTrans);

	// �`��
	for (DWORD i = 0; i < m_pMesh->dwNumMaterials; i++) {
		D3DMATERIAL9	mtrl = m_pMesh->pMaterials[i];
		mtrl.Diffuse.a *= m_alpha;
		mtrl.Diffuse.r = rangeCheck(mtrl.Diffuse.r + m_r);
		mtrl.Diffuse.g = rangeCheck(mtrl.Diffuse.g + m_g);
		mtrl.Diffuse.b = rangeCheck(mtrl.Diffuse.b + m_b);
		pd3dDevice->SetMaterial(&mtrl);							// �}�e���A���ݒ�
		pd3dDevice->SetTexture(0, m_pMesh->pTextures[i]);		// �e�N�X�`���ݒ�
		m_pMesh->pMesh->DrawSubset(i);							// ���b�V���`��
	}
}

//******************************************************************************
