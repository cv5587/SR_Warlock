#include "stdafx.h"
#include "..\Header\ExitMonster.h"
#include "MonsterBullet.h"

CExitMonster::CExitMonster(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev)
{
}

CExitMonster::CExitMonster(const CExitMonster& rhs)
    : CMonster(rhs)
{

}

CExitMonster::~CExitMonster()
{
}

HRESULT CExitMonster::Ready_GameObject()
{

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iMaxHealth = m_iCurHealth = 100;
    m_iDamage = 5;
    m_fSpeed = 1.f;
    m_fDtctRng = 2.f;
    m_fAtkRng = 5.f;
    m_bInit = false;
    m_bHit = false;
    m_bAtk = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 3.f;
    
    m_pTransformCom->Set_Pos({ -32.8f, 2.1f, 11.f });
    m_pTransformCom->Set_Angle({ 0.f, D3DXToRadian(90.f), 0.f });

    _vec3 vPos = m_vPrePos = m_pTransformCom->Get_Pos();
    m_szName = L"Exit";

    m_eState = MONSTER_IDLE;
    m_ePreState = MONSTER_END;

    m_fRadius = 0.5f;
    m_lowerBound = { vPos.x - m_fRadius, vPos.y - m_fRadius ,vPos.z - m_fRadius };
    m_upperBound = { vPos.x + m_fRadius, vPos.y + m_fRadius ,vPos.z + m_fRadius };

    m_eObjType = OT_MONSTER;
   // CGameObject::Push_CollisionMgr(m_eObjType, m_fRadius);

    return S_OK;
}

HRESULT CExitMonster::Ready_GameObject(D3DXVECTOR3 vPos)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iMaxHealth = m_iCurHealth = 100;
    m_iDamage = 5;
    m_fSpeed = 1.f;
    m_fDtctRng = 2.f;
    m_fAtkRng = 15.f;
    m_bInit = false;
    m_bHit = false;
    m_bAtk = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 3.f;
    m_szName = L"Exit";

    m_pTransformCom->Set_Pos(vPos);
    m_vPrePos = vPos;

    m_eState = MONSTER_IDLE;
    m_ePreState = MONSTER_END;

    m_fRadius = 5.f;
    m_eObjType = OT_MONSTER;

    m_lowerBound = { vPos.x - m_fRadius, vPos.y - m_fRadius ,vPos.z - m_fRadius };
    m_upperBound = { vPos.x + m_fRadius, vPos.y + m_fRadius ,vPos.z + m_fRadius };

    m_bColliderIsNone = true;


    return S_OK;
}

Engine::_int CExitMonster::Update_GameObject(const _float& fTimeDelta)
{

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    CGameObject::Update_GameObject(fTimeDelta);

    m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

    //버그 있으면 수정요청 바람..
    if (m_fFrame > _float(m_iMaxFrame) && m_eState == MONSTER_ATK)
    {
        m_bAtk = true;
        m_fFrame = 0.f;
    }
    else if (m_fFrame > _float(m_iMaxFrame) && m_eState != MONSTER_DEAD)
        m_fFrame = 0.f;

    if (m_bHit)
    {
        ExitMonster_Take_Dmg(fTimeDelta);
    }

    switch (m_eState)
    {
    case MONSTER_IDLE:
    {
        if (!m_bInit)
        {
            m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
            m_bInit = true;
        }

        if (m_fDtctRng > DistanceToPlayer(m_pPlayer))
        {
            m_eState = MONSTER_ATK;
        }
    }
    break;
    case MONSTER_RUN:
    {
        if (m_fAtkRng > DistanceToPlayer(m_pPlayer))
        {
            m_eState = MONSTER_ATK;

        }
        else
        {
            m_eState = MONSTER_IDLE;
        }

    }
    break;
    case MONSTER_ATK:
    {
        if (m_bAtk)
        {
            //총알 생성
            CGameObject* pGameObject;
            _vec3 vFrom;
            vFrom = m_pTransformCom->Get_Pos();

            pGameObject = CMonsterBullet::Create(m_pGraphicDev, BULLET_LOOKER, vFrom, m_pPlayer->Get_TransformCom_Pointer()->Get_Pos());

            if (pGameObject != nullptr)
            {
                _vec3 vDir;
                D3DXVec3Normalize(&vDir, &(m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vFrom));
                CTransform* transf = dynamic_cast<CTransform*>(pGameObject->Get_TransformCom_Pointer());
                transf->Set_Pos((vFrom + vDir * 0.1f));
                transf->Set_Scale({ 0.6f, 0.6f, 0.6f });

                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"LookerBullet", pGameObject);
            }
            m_bAtk = false;
        }



    }
    break;
    case MONSTER_HIT:
    {
        m_fStiffTimer += 10 * fTimeDelta;
        if (30.f < m_fStiffTimer)
        {
            m_fStiffTimer = 0.f;
            m_eState = MONSTER_IDLE;
        }
    }
    break;
    case MONSTER_DEAD:
    {
        if (m_iMaxFrame <= (_uint)(m_fFrame))
        {
            //TODO : 매끄러운 사망 구현
            return OBJ_DEAD;
        }

    }
    break;
    }

    return 0;
}

void CExitMonster::LateUpdate_GameObject(const float& fTimeDelta)
{

    Motion_Change();
    __super::LateUpdate_GameObject(fTimeDelta);

}

void CExitMonster::Render_GameObject()
{
    if (m_eState == MONSTER_DEAD && m_iMaxFrame <= (_uint)(m_fFrame))
    {
        return;
    }

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);


    D3DXVECTOR3 tmp = m_pTransformCom->Get_Pos();
    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CExitMonster::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    if (pGameObject->Get_ObjType() == OT_PLAYER_BULLET)
    {
        Take_Dmg(dynamic_cast<CPlayerBullet*>(pGameObject)->Get_Dmg());

        Take_Dmg(dynamic_cast<CPlayerBullet*>(pGameObject)->Get_Dmg());

        //피격 이펙트 출력
        CGameObject* pEffect;
        _vec3 vPos = pGameObject->Get_TransformCom_Pointer()->Get_Pos();
        pEffect = CEffect::Create(m_pGraphicDev, FX_BLOOD_RED, vPos);

        if (pEffect != nullptr)
        {
            //이펙트 플레이어 방향으로 조금 이동
            _vec3 vDir;
            D3DXVec3Normalize(&vDir, &(m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vPos));
            CTransform* transf = dynamic_cast<CTransform*>(pEffect->Get_TransformCom_Pointer());
            transf->Set_Pos((vPos + vDir * 0.1f));
            //이펙트 크기 조정
            transf->Set_Scale({ 0.5f, 0.5f, 0.5f });
            CGameObject* pParticleObject;
            pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.08f, 6, 0);
            Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Firework", pParticleObject);
            Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"ShamblerEffect", pEffect);
        }
    }
    __super::Action_On_Collision(pGameObject, fTimeDelta);
}

void CExitMonster::Action_On_Collision(vector<CLine*> pvecLines)
{
}

HRESULT CExitMonster::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    //Textures
    pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExitMonsterIdleTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ExitMonsterIdleTexture", pComponent });

    pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExitMonsterAtkTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ExitMonsterAtkTexture", pComponent });

    pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExitMonsterHitTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ExitMonsterHitTexture", pComponent });

    pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExitMonsterDeadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ExitMonsterDeadTexture", pComponent });
    //Texture Ends

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

CExitMonster* CExitMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CExitMonster* pInstance = DBG_NEW CExitMonster(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

CExitMonster* CExitMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos)
{
    CExitMonster* pInstance = DBG_NEW CExitMonster(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject(vPos)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CExitMonster::Free()
{
    CMonster::Free();
}

void CExitMonster::Motion_Change()
{
    if (m_eState != m_ePreState)
    {
        switch (m_eState)
        {
        case MONSTER_IDLE:
        case MONSTER_RUN:
        {
            m_pTextureCom = m_pTexCom_Atk;
            m_iMaxFrame = 6;
            m_fFrameSpeed = 10.f;
        }
        break;
        case MONSTER_ATK:
        {
            m_pTextureCom = m_pTexCom_Atk;
            m_iMaxFrame = 8;
            m_fFrameSpeed = 5.f;
        }
        break;
        case MONSTER_HIT:
        {
            m_pTextureCom = m_pTexCom_Hit;
            m_iMaxFrame = 1;
            m_fFrameSpeed = 1.f;
        }
        break;
        case MONSTER_DEAD:
        {
            CGameObject::Remove_Collider();
            m_pTextureCom = m_pTexCom_Dead;
            m_iMaxFrame = 25;
            m_fFrameSpeed = 10.f;
        }
        break;
        }

        m_ePreState = m_eState;

    }

}

void CExitMonster::ExitMonster_Take_Dmg(const _float& fTimeDelta)
{
    if (m_eState != MONSTER_DEAD)
    {
        if (0 < m_iCurHealth)
        {
            m_eState = MONSTER_HIT;
        }
        else if (0 >= m_iCurHealth)
        {
            m_eState = MONSTER_DEAD;
            m_fFrame = 0.f;
        }
        m_bHit = false;
    }
}