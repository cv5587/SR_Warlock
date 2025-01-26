#include "stdafx.h"
#include "..\Header\Prop.h"

#include "Export_System.h"
#include "Export_Utility.h"

CProp::CProp(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CProp::CProp(const CProp& rhs)
	: Engine::CGameObject(rhs)
{

}

CProp::~CProp()
{
}

HRESULT CProp::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->m_vScale = { 2.f, 1.f, 1.f };


	return S_OK;
}

Engine::_int CProp::Update_GameObject(const _float& fTimeDelta)
{


	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CProp::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);

}

void CProp::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

}

void CProp::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
}

void CProp::Action_On_Collision(vector<CLine*> pvecLines)
{
}

HRESULT CProp::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	pComponent  = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CProp::TexNamer()
{
}

CProp* CProp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProp* pInstance = DBG_NEW CProp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Prop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CProp::Free()
{
	CGameObject::Free();
}
