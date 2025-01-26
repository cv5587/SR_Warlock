#include "stdafx.h"
#include "LineList.h"

#include "Export_Utility.h"

CLineList::CLineList(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CLineList::CLineList(const CLineList& rhs)
	: Engine::CGameObject(rhs)
{
	Ready_GameObject();
}

CLineList::~CLineList()
{
}

HRESULT CLineList::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CLineList::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CLineList::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CLineList::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//For Debuging

	D3DXMATRIX matTotal;
	D3DXMatrixIdentity(&matTotal);

	for (auto& iter : m_pvecTransformCom)
	{

	}


	for (auto& iter : m_pvecLineGroupCom)
	{
		if (E_FAIL == (iter->Render_Buffer()))
		{
			int i = 0;
		}
	}

}

HRESULT CLineList::Add_Component()
{
	TCHAR szProtoName[128] = L"Proto_Line%d";
	TCHAR szProtoTName[128] = L"Proto_Transform%d";

	for (int i = 0; i < 260; ++i)
	{

		TCHAR	szName[128] = L"";

		wsprintf(szName, szProtoName, i);

		CComponent* pComponent = nullptr;
		CLine* pLine = nullptr;

		pComponent = pLine = dynamic_cast<CLine*>(Engine::Clone_Proto(szName));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ szName, pComponent });
		m_pvecLineGroupCom.push_back(pLine);

		

		CTransform* pTransformCom = nullptr;
		pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pTransformCom, E_FAIL);

		TCHAR	szTName[128] = L"";
		wsprintf(szTName, szProtoTName, i);

		m_mapComponent[ID_DYNAMIC].insert({ szTName, pComponent});
		m_pvecTransformCom.push_back(pTransformCom);
	}

	return S_OK;
}

CLineList* CLineList::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLineList* pInstance = new CLineList(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("LineList Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLineList::Free()
{
	for (int i = 0; i < m_pvecLineGroupCom.size(); ++i)
	{
		Safe_Release(m_pvecLineGroupCom[i]);
	}
	for (int i = 0; i < m_pvecTransformCom.size(); ++i)
	{
		Safe_Release(m_pvecTransformCom[i]);
	}
}
