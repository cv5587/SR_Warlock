#include "stdafx.h"
#include "Staff.h"

#include <PB_Staff.h>

CStaff::CStaff(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CStaff::CStaff(const CStaff& rhs)
	:CWeapon(rhs)
{
}

CStaff::~CStaff()
{
}

HRESULT CStaff::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 50;
	m_iMaxBullet = 50;
	m_iDamage = 15;
	return S_OK;
}

HRESULT CStaff::Ready_Clone_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CStaff::Update_GameObject(const _float& fTimeDelta)
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

void CStaff::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CStaff::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

void CStaff::Attack(_vec3 vPos, _vec3 vDir)
{
	if (!m_bAttack && m_iCurBullet > 0)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Shoot_Staff.wav", EFFECT, 0.27f);
		m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_SHOOT, m_iLightIndex, true, vPos - vDir, 3.f);
		m_fLightFrame = m_fFrame;
		--m_iCurBullet;
		m_bAttack = true;
		CPB_Staff* pBullet = CPB_Staff::Create(m_pGraphicDev, m_iDamage);
		pBullet->Set_Dir(vDir);
		pBullet->Set_PrePos(vPos + vDir);
		pBullet->Set_Pos(vPos + vDir);
		Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_Staff", pBullet);
		_vec3 vUp, vRight;
		vUp = { 0.f, 1.f, 0.f };
		D3DXVec3Cross(&vRight, &vUp, &vDir);
		D3DXVec3Cross(&vUp, &vDir, &vRight);
	}
	else if (m_iCurBullet == 0)
	{
		Engine::Get_SoundMgr()->Play_Sound(L"Bullet_Empty.wav", PROPSOUND, 0.5f, false);
	}
}

HRESULT CStaff::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Staff"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Staff", pComponent });

	return S_OK;
}

CStaff* CStaff::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaff* pInstance = DBG_NEW CStaff(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Staff Create Failed");
		return nullptr;
	}

	return pInstance;
}

CStaff* CStaff::Clone_Create(const CStaff& rhs)
{
	CStaff* pInstance = DBG_NEW CStaff(rhs);

	if (FAILED(pInstance->Ready_Clone_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Staff Clone Failed");
		return nullptr;
	}

	return pInstance;
}

void CStaff::Free()
{
	__super::Free();
}