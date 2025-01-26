#include "stdafx.h"
#include "..\Header\PB_Rocket.h"
#include "Export_Utility.h"
#include <Monster.h>

CPB_Rocket::CPB_Rocket(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
	: CPlayerBullet(pGraphicDev, _iDmg, _fSpeed)
{
	m_ePBType = PB_ROCKET;
}

CPB_Rocket::CPB_Rocket(const CPB_Rocket& rhs)
	: CPlayerBullet(rhs)
{
	m_ePBType = PB_ROCKET;
}

CPB_Rocket::~CPB_Rocket()
{
}

HRESULT CPB_Rocket::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Push_CollisionMgr(OT_PLAYER_BULLET, 0.1f);
	m_fSpeed = 10.f;
	return S_OK;
}

int CPB_Rocket::Update_GameObject(const float& fTimeDelta)
{
	_int iReturn = __super::Update_GameObject(fTimeDelta);
	if ((_int)m_fFrame % 5 == 4)
	{
		_vec3 vPos = m_pTransformCom_Clone->Get_Pos();
		_vec3 vRand = { (rand() % 5 + 1) / 10.f, (rand() % 5 + 1) / 10.f - 0.3f, (rand() % 5 + 1) / 10.f };
	
		Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_Rocket", CSmoke::Create(m_pGraphicDev, vPos + vRand));
	}
	return iReturn;
}

void CPB_Rocket::Render_GameObject()
{
	// 3Ãà ºôº¸µå
	_vec3 vPos;
	_matrix	matView;
	vPos = m_pTransformCom->Get_Pos();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	matView._41 = vPos.x;
	matView._42 = vPos.y;
	matView._43 = vPos.z;
	m_pTransformCom->Set_WorldMatrix(&(matView));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(1);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPB_Rocket::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
	OBJ_TYPE ot = pGameObject->Get_ObjType();
	if (pGameObject->Get_ObjType() == OT_MONSTER)
	{
		m_vIntersectionPoint = Calculate_Hit_Point(static_cast<CMonster*>(pGameObject));

		//dynamic_cast<CMonster*>(pGameObject)->Take_Dmg(m_iDmg);
		m_bDead = true;
	}
	else if (ot == OT_NONPASSPROP && m_bDead == false)
	{
		m_vIntersectionPoint = Calculate_Hit_Point(static_cast<CMonster*>(pGameObject));
		m_bDead = true;
	}
}

void CPB_Rocket::Action_On_Collision(vector<CLine*> pvecLines)
{
	m_vIntersectionPoint = Calculate_Hit_Point(pvecLines.front());
	m_bDead = true;
}

CPB_Rocket* CPB_Rocket::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
{
	CPB_Rocket*	pInstance = DBG_NEW CPB_Rocket(pGraphicDev, _iDmg, _fSpeed);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("PB_Rocket Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPB_Rocket::Free()
{
	__super::Free();
}
