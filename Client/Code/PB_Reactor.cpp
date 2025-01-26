#include "stdafx.h"
#include "..\Header\PB_Reactor.h"
#include "Export_Utility.h"
#include <Monster.h>

CPB_Reactor::CPB_Reactor(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
    : CPlayerBullet(pGraphicDev, _iDmg, _fSpeed), m_bDivided(false), m_bAttack(true), m_fFrame(0.f)
{
    m_ePBType = PB_REACTOR;
}

CPB_Reactor::CPB_Reactor(const CPB_Reactor& rhs)
    : CPlayerBullet(rhs), m_bDivided(rhs.m_bDivided), m_bAttack(true), m_fFrame(0.f)
{
    m_ePBType = PB_REACTOR;
}

CPB_Reactor::~CPB_Reactor()
{
}

HRESULT CPB_Reactor::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    Push_CollisionMgr(OT_PLAYER_BULLET, 0.4f);
    m_fSpeed = 10.f;
    return S_OK;
}

int CPB_Reactor::Update_GameObject(const float& fTimeDelta)
{
    if (!m_bAttack)
    {
        if (m_fFrame > 0.f)
        {
            m_bAttack = true;
            CGameObject::Push_CollisionMgr();
        }
        else CGameObject::Remove_Collider();
    }
    m_fFrame += 90.f * fTimeDelta;
    _int iReturn = __super::Update_GameObject(fTimeDelta);
    if (iReturn == OBJ_DEAD)
    {
        m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_REACTOR, m_iLightIndex, FALSE);
        if (!m_bDivided)
        {
            Engine::Get_SoundMgr()->Play_Sound(L"Reactor_Divide.wav", EFFECT, 0.5f);

            _vec3 vRight, vDir, vPos;
            vDir = m_vDir;
            vPos = m_pTransformCom_Clone->Get_Pos();
            D3DXVec3Cross(&vRight, &_vec3({ 0.f, 1.f, 0.f }), &vDir);

            // 4방향 대각선으로 분열
            for (int i = 0; i < 4; i++)
            {
                CPB_Reactor* pDivBullet = CPB_Reactor::Create(m_pGraphicDev, m_iDmg, 10.f);
                pDivBullet->Set_Divided();
                pDivBullet->Set_Dir(_float(1 - (i / 2) * 2) * vDir + _float(1 - (i % 2) * 2) * vRight);
                pDivBullet->Set_Pos(vPos);
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_Reactor", pDivBullet);
            }
        }
    }
    return iReturn;
}

void CPB_Reactor::Render_GameObject()
{
    // 3축 빌보드
    _vec3 vPos = m_pTransformCom_Clone->Get_Pos();
    _matrix   matView;
    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matView, NULL, &matView);
    matView._41 = vPos.x;
    matView._42 = vPos.y;
    matView._43 = vPos.z;
    _matrix matScale;
    D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.7f);
    matView = matScale * matView;
    m_pTransformCom->Set_WorldMatrix(&(matView));

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (!m_bDivided) m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_REACTOR, m_iLightIndex, true, vPos, 3.f);

    Engine::Enable_Material(m_pGraphicDev, MTRL_REACTOR);

    m_pTextureCom->Set_Texture(((_int)m_fFrame % 40) / 5 + 3);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    Engine::Enable_Material(m_pGraphicDev);
}

void CPB_Reactor::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    if (!m_bAttack) return;
    OBJ_TYPE ot = pGameObject->Get_ObjType();
    if (ot == OT_MONSTER)
    {
        m_vIntersectionPoint = Calculate_Hit_Point(static_cast<CMonster*>(pGameObject));
        m_fFrame = -30.f;
        m_bAttack = false;
    }
    else if (ot == OT_NONPASSPROP && m_bDead == false)
    {
        m_vIntersectionPoint = Calculate_Hit_Point(static_cast<CMonster*>(pGameObject));
    }
}

void CPB_Reactor::Action_On_Collision(vector<CLine*> pvecLines)
{
    m_vIntersectionPoint = Calculate_Hit_Point(pvecLines.front());
    m_bDead = true;
}

CPB_Reactor* CPB_Reactor::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed)
{
    CPB_Reactor* pInstance = DBG_NEW CPB_Reactor(pGraphicDev, _iDmg, _fSpeed);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("PB_Reactor Create Failed");
        return nullptr;
    }

    return pInstance;
}