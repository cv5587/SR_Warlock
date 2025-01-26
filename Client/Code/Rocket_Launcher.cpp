#include "stdafx.h"
#include "Rocket_Launcher.h"

#include "PB_Rocket.h"

CRocket_Launcher::CRocket_Launcher(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CRocket_Launcher::CRocket_Launcher(const CRocket_Launcher& rhs)
	:CWeapon(rhs)
{
}

CRocket_Launcher::~CRocket_Launcher()
{
}

HRESULT CRocket_Launcher::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 15;
	m_iMaxBullet = 15;
	m_iDamage = 50;
	m_iUpgrade_cost = 20;
	return S_OK;
}

HRESULT CRocket_Launcher::Ready_Clone_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	if (m_bUpgraded) Upgrade_Weapon();
	return S_OK;
}

_int CRocket_Launcher::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (m_bAttack)
	{
		if (180.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bAttack = false;
		}
		else
		{
			m_fFrame += 90.f * fTimeDelta;
		}
	}

	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CRocket_Launcher::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CRocket_Launcher::Render_GameObject()
{
	if (m_fFrame > 30.f) m_pTextureCom->Set_Texture(0);
	else m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

void CRocket_Launcher::Attack(_vec3 vPos, _vec3 vDir)
{
	if (!m_bAttack && m_iCurBullet > 0)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Shoot_Rocket_Launcher.wav", EFFECT, 0.27f);
		m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_SHOOT, m_iLightIndex, true, vPos - vDir, 3.f);
		m_fLightFrame = m_fFrame;
		--m_iCurBullet;
		m_bAttack = true;
		CPB_Rocket* pBullet = CPB_Rocket::Create(m_pGraphicDev, m_iDamage);
		pBullet->Set_Dir(vDir);
		pBullet->Set_PrePos(vPos + vDir);
		pBullet->Set_Pos(vPos + vDir);
		Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_Rocket", pBullet);
	}
	else if (m_iCurBullet == 0)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Bullet_Empty.wav", PROPSOUND, 0.5f, false);
	}
}

void CRocket_Launcher::Upgrade_Weapon()
{
	m_bUpgraded = true;
	m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Napalm_Launcher"));
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Napalm_Launcher", m_pTextureCom });
	m_iCurBullet = 30;
	m_iMaxBullet = 30;
	m_iDamage = 70;
}

HRESULT CRocket_Launcher::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Rocket_Launcher"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Rocket_Launcher", pComponent });

	return S_OK;
}

CRocket_Launcher* CRocket_Launcher::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRocket_Launcher* pInstance = DBG_NEW CRocket_Launcher(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Rocket_Launcher Create Failed");
		return nullptr;
	}

	return pInstance;
}

CRocket_Launcher* CRocket_Launcher::Clone_Create(const CRocket_Launcher& rhs)
{
	CRocket_Launcher* pInstance = DBG_NEW CRocket_Launcher(rhs);

	if (FAILED(pInstance->Ready_Clone_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Rocket_Launcher Clone Failed");
		return nullptr;
	}

	return pInstance;
}

void CRocket_Launcher::Free()
{
	__super::Free();
}