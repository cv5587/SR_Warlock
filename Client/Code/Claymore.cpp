#include "stdafx.h"
#include "Claymore.h"

#include <PB_Claymore.h>

CClaymore::CClaymore(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CClaymore::CClaymore(const CClaymore& rhs)
	:CWeapon(rhs)
{
}

CClaymore::~CClaymore()
{
}

HRESULT CClaymore::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 0;
	m_iMaxBullet = 0;
	m_iDamage = 50;
	return S_OK;
}

HRESULT CClaymore::Ready_Clone_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CClaymore::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	if (m_bAttack)
	{
		if (65.f < m_fFrame)
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

void CClaymore::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CClaymore::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

void CClaymore::Attack(_vec3 vPos, _vec3 vDir)
{
	if (!m_bAttack)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Shoot_Claymore.wav", EFFECT, 0.27f);
		m_bAttack = true;
		CPB_Claymore* pBullet = CPB_Claymore::Create(m_pGraphicDev, m_iDamage);
		pBullet->Set_Dir({ 0.f, 0.f, 0.f });
		pBullet->Set_PrePos(vPos + vDir * 0.5f);
		pBullet->Set_Pos(vPos + vDir * 0.5f);
		Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_Claymore", pBullet);
	}
}

HRESULT CClaymore::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Claymore"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Claymore", pComponent });

	pComponent = m_pUITextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIWeapon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UIWeapon", pComponent });

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CClaymore* CClaymore::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CClaymore* pInstance = DBG_NEW CClaymore(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Claymore Create Failed");
		return nullptr;
	}

	return pInstance;
}

CClaymore* CClaymore::Clone_Create(const CClaymore& rhs)
{
	CClaymore* pInstance = DBG_NEW CClaymore(rhs);

	if (FAILED(pInstance->Ready_Clone_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Claymore Clone Failed");
		return nullptr;
	}

	return pInstance;
}

void CClaymore::Free()
{
	__super::Free();
}