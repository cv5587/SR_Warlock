#include "stdafx.h"
#include "BreakableProp.h"

CBreakableProp::CBreakableProp(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID)
	: CProp(pGraphicDev), m_bDead(false), m_eBoxID(eBoxID)
{
}

CBreakableProp::CBreakableProp(const CBreakableProp& rhs)
	:	CProp(rhs), m_eBoxID(rhs.m_eBoxID), m_bDead(rhs.m_bDead)
{
}

CBreakableProp::~CBreakableProp()
{
}

HRESULT CBreakableProp::Ready_GameObject()
{
	return E_NOTIMPL;
}

_int CBreakableProp::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CBreakableProp::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CBreakableProp::Render_GameObject()
{
}

void CBreakableProp::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
}

void CBreakableProp::Action_On_Collision(vector<CLine*> pvecLines)
{
}

CBreakableProp* CBreakableProp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CBreakableProp::Free()
{
	CProp::Free();
}

HRESULT CBreakableProp::Add_Component()
{
	return E_NOTIMPL;
}

void CBreakableProp::TexNamer()
{
}
