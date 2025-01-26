#include "stdafx.h"
#include "..\Header\PB_Staff.h"
#include "Export_Utility.h"
#include <Monster.h>

CPB_Staff::CPB_Staff(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
	: CPlayerBullet(pGraphicDev, _iDmg, _fSpeed)
{
	m_ePBType = PB_STAFF;
}

CPB_Staff::CPB_Staff(const CPB_Staff& rhs)
	: CPlayerBullet(rhs)
{
	m_ePBType = PB_STAFF;
}

CPB_Staff::~CPB_Staff()
{
}

void CPB_Staff::Render_GameObject()
{
	// 3Ãà ºôº¸µå
	_vec3 vPos;
	_matrix	matWorld, matView, matBill;
	vPos = m_pTransformCom_Clone->Get_Pos();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	D3DXMatrixInverse(&matView, NULL, &matView);
	matView._41 = vPos.x;
	matView._42 = vPos.y;
	matView._43 = vPos.z;
	m_pTransformCom->Set_WorldMatrix(&(matView));


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(2);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPB_Staff::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
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

void CPB_Staff::Action_On_Collision(vector<CLine*> pvecLines)
{
	m_vIntersectionPoint = Calculate_Hit_Point(pvecLines.front());
	m_bDead = true;
}

CPB_Staff * CPB_Staff::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
{
	CPB_Staff *	pInstance = DBG_NEW CPB_Staff(pGraphicDev, _iDmg, _fSpeed);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("PB_Staff Create Failed");
		return nullptr;
	}

	return pInstance;
}