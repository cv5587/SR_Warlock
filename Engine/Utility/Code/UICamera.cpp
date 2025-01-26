#include "UICamera.h"

CUICamera::CUICamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUICamera::~CUICamera()
{
}

HRESULT CUICamera::Ready_UICamera()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, 960.f, 540.f, 0.f, 1000.f);

	return S_OK;
}

_int CUICamera::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CUICamera::LateUpdate_GameObject(const float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CUICamera::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&m_matWorld));
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	__super::Render_GameObject();
}

void CUICamera::Free()
{
	CGameObject::Free();
}