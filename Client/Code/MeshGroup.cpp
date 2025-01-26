#include "stdafx.h"
#include "MeshGroup.h"

#include "Export_Utility.h"

int CMeshGroup::m_iProtoMeshIdx = 0;
int CMeshGroup::m_iProtoDMeshIdx = 0;

CMeshGroup::CMeshGroup(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pTransformCom(nullptr), m_bIsClone(false)
{
}

CMeshGroup::CMeshGroup(const CMeshGroup& rhs, const std::vector<FbxMeshInfo>& vMeshGroup, const std::vector<FbxMeshInfo>& vDMeshGroup)
	: Engine::CGameObject(rhs), m_bIsClone(true)
{
}

CMeshGroup::~CMeshGroup()
{
}

HRESULT CMeshGroup::Ready_GameObject(const std::vector<FbxMeshInfo>& vMeshGroup, const std::vector<FbxMeshInfo>& vDMeshGroup)
{
	FAILED_CHECK_RETURN(Add_Component(vMeshGroup,vDMeshGroup), E_FAIL);
	return S_OK;
}

_int CMeshGroup::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CMeshGroup::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CMeshGroup::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	int cnt = 0;

	for (auto& pMesh : m_pvecMeshGroupCom)
	{

		if (cnt < m_pvecTextureGroupCom.size())
			m_pvecTextureGroupCom[cnt]->Set_Texture(m_vTextureIdx[cnt]);
		else
			m_pvecTextureGroupCom[0]->Set_Texture(0);

		pMesh->Render_Buffer();

		
		cnt++;
	}


}

HRESULT CMeshGroup::Add_Component(const std::vector<FbxMeshInfo>& vMeshGroup, const std::vector<FbxMeshInfo>& vDMeshGroup)
{
	TCHAR szProtoName[128] = L"Proto_Mesh%d";

	for (int i = 0; i < vMeshGroup.size(); ++i)
	{

		TCHAR	szName[128] = L"";

		wsprintf(szName, szProtoName, m_iProtoMeshIdx);

		CComponent* pComponent = nullptr;
		CMesh* pMesh = nullptr;

		pComponent = pMesh = dynamic_cast<CMesh*>(Engine::Clone_Proto(szName));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ szName, pComponent});
		m_pvecMeshGroupCom.push_back(pMesh);
		m_iProtoMeshIdx++;
		m_iCurProtoMeshSize++;
	}

	TCHAR szProtoName2[128] = L"Proto_DMesh%d";

	for (int i = 0; i < vDMeshGroup.size(); ++i)
	{
		TCHAR	szName[128] = L"";

		wsprintf(szName, szProtoName2, m_iProtoDMeshIdx);

		CComponent* pComponent = nullptr;
		CMesh* pMesh = nullptr;

		pComponent = pMesh = dynamic_cast<CMesh*>(Engine::Clone_Proto(szName));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ szName, pComponent });
		m_pvecMeshGroupCom.push_back(pMesh);
		m_iProtoDMeshIdx++;
		m_iCurProtoDMeshSize++;
	}

	TCHAR szTexName[128] = L"Proto_TerrainTexture0";

	for (int i = 0; i < vMeshGroup.size() + vDMeshGroup.size(); ++i)
	{
		CComponent* pComponent = nullptr;
		CTexture* pTextureCom = nullptr;

		pComponent = pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(szTexName));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ szTexName, pComponent});
		m_pvecTextureGroupCom.push_back(pTextureCom);
	}

	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}


CMeshGroup* CMeshGroup::Create(LPDIRECT3DDEVICE9 pGraphicDev, const std::vector<FbxMeshInfo>& vMeshGroup, const std::vector<FbxMeshInfo>& vDMeshGroup)
{
	CMeshGroup* pInstance = DBG_NEW CMeshGroup(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject(vMeshGroup, vDMeshGroup)))
	{
		Safe_Release(pInstance);
		MSG_BOX("MeshGroup Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMeshGroup::Free()
{
	for (int i = 0; i < m_pvecMeshGroupCom.size(); ++i)
	{
		Safe_Release(m_pvecMeshGroupCom[i]);
	}
	for (int i = 0; i < m_pvecTextureGroupCom.size(); ++i)
	{
		Safe_Release(m_pvecTextureGroupCom[i]);
	}

	CGameObject::Free();
}
