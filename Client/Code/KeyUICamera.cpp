#include "stdafx.h"
#include "KeyUICamera.h"

#include "Export_System.h"

CKeyUICamera::CKeyUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer, DOORID eColor)
	:CUICamera(pGraphicDev), m_pPlayer(pPlayer), m_eColor(eColor)
{
}

CKeyUICamera::CKeyUICamera(const CKeyUICamera& rhs)
	:CUICamera(rhs), m_eColor(rhs.m_eColor)
{
}

CKeyUICamera::~CKeyUICamera()
{
}

HRESULT CKeyUICamera::Ready_GameObject(const _float& fWidth, const _float& fHeight,
	const _float& fPosX, const _float& fPosY)
{
	Engine::CUICamera::Ready_UICamera();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 맨 위에 출력하기 위해 값 설정
	m_iSortOrder = 997;
	m_pTransformCom->m_vScale.x = fWidth * 0.5f;
	m_pTransformCom->m_vScale.y = fHeight * 0.5f;
	m_pTransformCom->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

	return S_OK;
}

_int CKeyUICamera::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CKeyUICamera::LateUpdate_GameObject(const float& fTimeDelta)
{
	Engine::CUICamera::LateUpdate_GameObject(fTimeDelta);
}

void CKeyUICamera::Render_GameObject()
{
	if (!m_pPlayer->Get_Key(m_eColor)) return;

	Engine::CUICamera::Render_GameObject();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_eColor);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CKeyUICamera::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom_Clone =  m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Key"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Key", pComponent });

	return S_OK;
}

CKeyUICamera* CKeyUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fWidth,
	const _float& fHeight, const _float& fPosX,	const _float& fPosY, CPlayer*& pPlayer, 
	DOORID eColor)
{
	CKeyUICamera* pInstance = DBG_NEW CKeyUICamera(pGraphicDev, pPlayer, eColor);

	if (FAILED(pInstance->Ready_GameObject(fWidth, fHeight, fPosX, fPosY)))
	{
		Safe_Release(pInstance);
		MSG_BOX("KeyUICamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CKeyUICamera::Free()
{
	Engine::CUICamera::Free();
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}