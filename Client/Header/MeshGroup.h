#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CMesh;
class CTexture;
class CTransform;

END

class CMeshGroup :
    public CGameObject
{
public:
	explicit CMeshGroup(const CMeshGroup& rhs, const std::vector<FbxMeshInfo>& vMeshGroup, const std::vector<FbxMeshInfo>& vDMeshGroup);

private:
	explicit CMeshGroup(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeshGroup();

public:
	virtual HRESULT Ready_GameObject(const std::vector<FbxMeshInfo>& vMeshGroup, const std::vector<FbxMeshInfo>& vDMeshGroup);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	 override;
	virtual void Render_GameObject()						 override;

	

private:
	HRESULT			Add_Component(const std::vector<FbxMeshInfo>& vMeshGroup, const std::vector<FbxMeshInfo>& vDMeshGroup);

public:
	vector<Engine::CMesh*>* Get_MeshGroup() { return &m_pvecMeshGroupCom; }
	vector<Engine::CTexture*>* Get_TextureGroup() { return &m_pvecTextureGroupCom; }

	void	Reserve_vecTexIdx(unsigned int number) { if (number > m_vTextureIdx.size()) { m_vTextureIdx.resize(number,0); } }
	void	Set_TexIdx(unsigned int index, unsigned int number) { m_vTextureIdx[index] = number; }

	
	void	Set_Is_Clone(bool bIsClone) { m_bIsClone = bIsClone; }
	bool	Is_Clone() { return m_bIsClone; }

	int Get_ProtoMeshSize() { return m_iProtoMeshIdx; }
	int Get_ProtoDMeshSize() { return m_iProtoDMeshIdx; }

	int Get_CurProtoMeshSize() { return m_iCurProtoMeshSize; }
	int Get_CurProtoDMeshSize() { return m_iCurProtoDMeshSize; }

private:
	vector<Engine::CMesh*> m_pvecMeshGroupCom;
	vector<Engine::CTexture*> m_pvecTextureGroupCom;
	Engine::CTransform* m_pTransformCom;
	vector<unsigned int> m_vTextureIdx;
	bool m_bIsClone;

	int	m_iCurProtoMeshSize = 0;
	int	m_iCurProtoDMeshSize = 0;


	static int m_iProtoMeshIdx;
	static int m_iProtoDMeshIdx;

public:
	static CMeshGroup* Create(LPDIRECT3DDEVICE9 pGraphicDev, const std::vector<FbxMeshInfo>& vMeshGroup, const std::vector<FbxMeshInfo>& vDMeshGroup);

private:
	virtual void Free() override;
};

