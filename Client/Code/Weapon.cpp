#include "stdafx.h"
#include "Weapon.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iCurBullet(0), m_iMaxBullet(0), m_pBufferCom(nullptr),m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_iDamage(0), m_bAttack(false), m_fFrame(0.f), m_bUpgraded(false), m_iUpgrade_cost(0), m_fLightFrame(0.f)
{
}

CWeapon::CWeapon(const CWeapon& rhs)
	: Engine::CGameObject(rhs), m_iCurBullet(rhs.m_iCurBullet), m_iMaxBullet(rhs.m_iMaxBullet)
	, m_iDamage(rhs.m_iDamage), m_bAttack(false), m_fFrame(0.f), m_bUpgraded(rhs.m_bUpgraded), m_iUpgrade_cost(rhs.m_iUpgrade_cost),
		m_fLightFrame(0.f)
{
}

CWeapon::~CWeapon()
{
}

_int CWeapon::Update_GameObject(const float& fTimeDelta)
{
	return 0;
}

void CWeapon::LateUpdate_GameObject(const float& fTimeDelta)
{
	if (abs(m_fLightFrame - m_fFrame) > 9.f)
	{
		m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_SHOOT, m_iLightIndex, false);
	}
}

bool CWeapon::Get_Attack()
{
	return m_bAttack;
}

void CWeapon::Free()
{
	__super::Free();
}