#include "stdafx.h"
#include "Revolver.h"

#include "PB_None.h"

CRevolver::CRevolver(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CRevolver::CRevolver(const CRevolver& rhs)
	:CWeapon(rhs)
{
}

CRevolver::~CRevolver()
{
}

HRESULT CRevolver::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 30;
	m_iMaxBullet = 30;
	m_iDamage = 20;
	m_iUpgrade_cost = 8;
	return S_OK;
}

HRESULT CRevolver::Ready_Clone_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	if (m_bUpgraded) Upgrade_Weapon();
	return S_OK;
}

_int CRevolver::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	if (m_bAttack)
	{
		if (20.f < m_fFrame)
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

void CRevolver::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CRevolver::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

void CRevolver::Upgrade_Weapon()
{
	m_bUpgraded = true;
	m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Magnum"));
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Magnum", m_pTextureCom });
	m_iDamage = 30;
	m_iCurBullet = 60;
	m_iMaxBullet = 60;
}

void CRevolver::Attack(_vec3 vPos, _vec3 vDir)
{
	if (!m_bAttack && m_iCurBullet > 0)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Shoot_Revolver.wav", EFFECT, 0.27f);
		m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_SHOOT, m_iLightIndex, true, vPos - vDir, 3.f);
		m_fLightFrame = m_fFrame;
		--m_iCurBullet;
		m_bAttack = true;
		CPB_None* pBullet = CPB_None::Create(m_pGraphicDev, m_iDamage);
		pBullet->Set_Dir(vDir);
		pBullet->Set_PrePos(vPos - 60.f * vDir * Get_TimeDelta(L"Timer_FPS60"));
		pBullet->Set_Pos(vPos - 60.f * vDir * Get_TimeDelta(L"Timer_FPS60"));
		Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_None", pBullet);
	}
	else if (m_iCurBullet == 0)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Bullet_Empty.wav", PROPSOUND, 0.5f, false);
	}
}

HRESULT CRevolver::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Revolver"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Revolver", pComponent });

	return S_OK;
}

CRevolver* CRevolver::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRevolver* pInstance = DBG_NEW CRevolver(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Revolver Create Failed");
		return nullptr;
	}

	return pInstance;
}

CRevolver* CRevolver::Clone_Create(const CRevolver& rhs)
{
	CRevolver* pInstance = DBG_NEW CRevolver(rhs);

	if (FAILED(pInstance->Ready_Clone_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Revolver Clone Failed");
		return nullptr;
	}

	return pInstance;
}

void CRevolver::Free()
{
	__super::Free();
}