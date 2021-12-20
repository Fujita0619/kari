#ifndef _GLMESH_H_
#define	_GLMESH_H_
//******************************************************************************
//
//
//		���b�V��
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

// ���b�V��
struct GLMesh
{
	LPD3DXMESH			pMesh;
	D3DMATERIAL9*		pMaterials;
	LPDIRECT3DTEXTURE9*	pTextures;
	DWORD				dwNumMaterials;

	GLMesh();
	~GLMesh();

	void load(LPDIRECT3DDEVICE9 pd3dDevice, char *filename);
	void release();
};

//==============================================================================
//
//		���\�[�X�Ǘ�
//
//==============================================================================

// ���b�V���Ǘ��N���X
class GLMeshManager
{
private:
	vector<GLMesh>	m_resource;
public:
	static GLMeshManager* getInstance()
	{
		static GLMeshManager instance;
		return &instance;
	}

	void Init();
	void Uninit();

	void Resize(int n);
	void Load(LPDIRECT3DDEVICE9 pd3dDevice, LOAD_MESH* pLoad);
	void ReleaseAll();

	GLMesh* getResource(int i);
};

#define pGLMeshManager		(GLMeshManager::getInstance())

//==============================================================================
//
//		�`��R�}���h
//
//==============================================================================

// ���b�V���`��
class GLDrawMesh : public IGLDrawCmd
{
private:
	GLMesh*			m_pMesh;
	D3DXMATRIX		m_matTrans;
	float			m_alpha;
	float			m_r;
	float			m_g;
	float			m_b;
public:
	GLDrawMesh(MESH_DATA* pMesh, D3DXMATRIX& matTrans, float alpha = 1.0f, float r = 0.0f, float g = 0.0f, float b = 0.0f);
	void execute(LPDIRECT3DDEVICE9 pd3dDevice);
};

//******************************************************************************
#endif // !_GLMESH_H_