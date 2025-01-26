#include "stdafx.h"
#include "TmpMesh.h"

#include "Export_Utility.h"

CTmpMesh::CTmpMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTmpMesh::CTmpMesh(const CTmpMesh& rhs)
	: Engine::CGameObject(rhs)
{
}

CTmpMesh::~CTmpMesh()
{
	CTmpMesh::Free();
}

HRESULT CTmpMesh::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTmpMesh::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CTmpMesh::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CTmpMesh::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//m_pTextureCom->Set_Texture(0);

	m_pMeshCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CTmpMesh::Add_Component()
{

	CComponent* pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<CMesh*>(Engine::Clone_Proto(L"Proto_Mesh11"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Mesh11", pComponent });
	return S_OK;
}


CTmpMesh* CTmpMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTmpMesh* pInstance = new CTmpMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTmpMesh::Free()
{
	__super::Free();
}
