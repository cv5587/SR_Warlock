#include "stdafx.h"
#include "Shotgun.h"

#include <PB_None.h>

CShotgun::CShotgun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev), m_bReload(false), m_bBulletCount(4)
{
}

CShotgun::CShotgun(const CShotgun& rhs)
	:CWeapon(rhs), m_bReload(false), m_bBulletCount(rhs.m_bBulletCount)
{
}

CShotgun::~CShotgun()
{
}

HRESULT CShotgun::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 20;
	m_iMaxBullet = 20;
	m_iDamage = 20;
	m_iUpgrade_cost = 10;
	return S_OK;
}

HRESULT CShotgun::Ready_Clone_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	if (m_bUpgraded) Upgrade_Weapon();
	return S_OK;
}

_int CShotgun::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	if (m_bAttack)
	{
		m_fFrame += 90.f * fTimeDelta;
		if (15.f < m_fFrame)
		{
			m_bAttack = false;
			m_bReload = true;
		}
	}
	else if (m_bReload)
	{
		m_fFrame += 90.f * fTimeDelta;
		if (!m_bUpgraded)
		{
			if (75.f < m_fFrame)
			{
				m_fFrame = 0.f;
				m_bReload = false;
			}
		}
		else
		{
			if (130.f < m_fFrame)
			{
				m_fFrame = 0.f;
				m_bReload = false;
			}
		}
	}
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CShotgun::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CShotgun::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

void CShotgun::Attack(_vec3 vPos, _vec3 vDir)
{
	if (!m_bAttack && !m_bReload && m_iCurBullet > 0)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Shoot_Shotgun.wav", EFFECT, 0.27f);
		m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_SHOOT, m_iLightIndex, true, vPos - vDir, 3.f);
		m_fLightFrame = m_fFrame;
		--m_iCurBullet;
		m_bAttack = true;
		_vec3 vLook, vUp, vRight, vNewDir;
		_vec3 vWorld_Up = { 0.f, 1.f, 0.f };

		D3DXVec3Normalize(&vLook, &vDir);

		D3DXVec3Cross(&vRight, &vWorld_Up, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXVec3Cross(&vUp, &vLook, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		for (int i = 0; i < m_bBulletCount; i++)
		{
			vNewDir = vLook + ((rand() % 20 - 10) / 100.f) * vUp + ((rand() % 40 - 20) / 100.f) * vRight;
			D3DXVec3Normalize(&vNewDir, &vNewDir);
			CPB_None* pBullet = CPB_None::Create(m_pGraphicDev, m_iDamage);
			pBullet->Set_Dir(vNewDir);
			pBullet->Set_PrePos(vPos - 60.f * vDir * Get_TimeDelta(L"Timer_FPS60"));
			pBullet->Set_Pos(vPos - 60.f * vDir * Get_TimeDelta(L"Timer_FPS60"));
			Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_None", pBullet);
		}
	}
	else if (m_iCurBullet == 0)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Bullet_Empty.wav", PROPSOUND, 0.5f, false);
	}
}

void CShotgun::Upgrade_Weapon()
{
	m_bUpgraded = true;
	m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Shotgun"));
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Shotgun", m_pTextureCom });
	m_bBulletCount = 6;
	m_iCurBullet = 40;
	m_iMaxBullet = 40;
}

HRESULT CShotgun::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_MiniShotgun"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_MiniShotgun", pComponent });

	return S_OK;
}

CShotgun* CShotgun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShotgun* pInstance = DBG_NEW CShotgun(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Shotgun Create Failed");
		return nullptr;
	}

	return pInstance;
}

CShotgun* CShotgun::Clone_Create(const CShotgun& rhs)
{
	CShotgun* pInstance = DBG_NEW CShotgun(rhs);

	if (FAILED(pInstance->Ready_Clone_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Shotgun Clone Failed");
		return nullptr;
	}

	return pInstance;
}

void CShotgun::Free()
{
	__super::Free();
}