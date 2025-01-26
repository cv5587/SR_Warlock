#include "stdafx.h"
#include "FaceUICamera.h"

#include "Export_System.h"

CFaceUICamera::CFaceUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer)
	:CUICamera(pGraphicDev), m_pPlayer(pPlayer)
{
}

CFaceUICamera::CFaceUICamera(const CFaceUICamera& rhs)
	:CUICamera(rhs)
{
}

CFaceUICamera::~CFaceUICamera()
{
}

HRESULT CFaceUICamera::Ready_GameObject(const _float& fWidth, const _float& fHeight, 
	const _float& fPosX, const _float& fPosY)
{
	Engine::CUICamera::Ready_UICamera();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 맨 위에 출력하기 위해 값 설정
	m_iSortOrder = 998;
	m_pTransformCom->m_vScale.x = fWidth * 0.5f;
	m_pTransformCom->m_vScale.y = fHeight * 0.5f;
	m_pTransformCom->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

	return S_OK;
}

_int CFaceUICamera::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += 90.f * fTimeDelta;

	if (240.f < m_fFrame)
		m_fFrame = 0.f;

	Engine::Add_RenderGroup(RENDER_UI, this);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CFaceUICamera::LateUpdate_GameObject(const float& fTimeDelta)
{
	Engine::CUICamera::LateUpdate_GameObject(fTimeDelta);
}

void CFaceUICamera::Render_GameObject()
{
	Engine::CUICamera::Render_GameObject();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_int ratio = (m_pPlayer->Get_MaxHealth() - m_pPlayer->Get_Health()) / 40;
	m_pTextureCom->Set_Texture(9 * ratio + (_uint)m_fFrame / 60);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFaceUICamera::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent  = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Face"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Face", pComponent });

	return S_OK;
}

CFaceUICamera* CFaceUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fWidth,
	const _float& fHeight, const _float& fPosX, const _float& fPosY, CPlayer*& pPlayer)
{
	CFaceUICamera* pInstance = DBG_NEW CFaceUICamera(pGraphicDev, pPlayer);

	if (FAILED(pInstance->Ready_GameObject(fWidth, fHeight, fPosX, fPosY)))
	{
		Safe_Release(pInstance);
		MSG_BOX("FaceUICamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFaceUICamera::Free()
{
	Engine::CUICamera::Free();

}