#include "stdafx.h"
#include "..\Header\PB_None.h"
#include "Export_Utility.h"
#include <Monster.h>

CPB_None::CPB_None(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
    : CPlayerBullet(pGraphicDev, _iDmg, _fSpeed)
{
    m_ePBType = PB_NONE;
}

CPB_None::CPB_None(const CPB_None& rhs)
    : CPlayerBullet(rhs)
{
    m_ePBType = PB_NONE;
}

CPB_None::~CPB_None()
{
}

void CPB_None::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    //m_pTextureCom->Set_Texture(0);
    //m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPB_None::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    OBJ_TYPE ot = pGameObject->Get_ObjType();
    if (ot == OT_MONSTER && m_bDead == false)
    {
        /*CGameObject* pExpl;
        _vec3 vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);
        pExpl = CExplosion::Create(m_pGraphicDev, vPos);

        if (pGameObject != nullptr)
        {
           Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BoxExplEffect", pExpl);
        }
        */

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

void CPB_None::Action_On_Collision(vector<CLine*> pvecLines)
{
    //CGameObject* pFire;
    //pFire = CExplosion::Create(m_pGraphicDev, m_pTransformCom->Get_Pos());

    //if (pFire != nullptr)
    //{
    //   //vPos = dynamic_cast<CTransform*>(pFire->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Pos();
    //   dynamic_cast<CTransform*>(pFire->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(m_pTransformCom->Get_Pos());
    //   Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Explosion", pFire);
    //}

    m_vIntersectionPoint = Calculate_Hit_Point(pvecLines.front());
    m_bDead = true;
}

CPB_None* CPB_None::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
{
    CPB_None* pInstance = DBG_NEW CPB_None(pGraphicDev, _iDmg, _fSpeed);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("PB_None Create Failed");
        return nullptr;
    }

    return pInstance;
}