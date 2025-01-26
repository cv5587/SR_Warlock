#include "stdafx.h"
#include "..\Header\Yeti.h"
#include "Export_System.h"


CYeti::CYeti(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev), m_fAccel(1.f), m_fInitSpeed(1.f), m_fMaxSpeed(30.f)
{
}

CYeti::CYeti(const CYeti& rhs)
    : CMonster(rhs), m_fMaxSpeed(30.f)
{

}

CYeti::~CYeti()
{

}

HRESULT CYeti::Ready_GameObject()
{

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iMaxHealth = m_iCurHealth = 100;
    m_iDamage = 5;
    m_fSpeed = 2.f;
    m_fInitSpeed = 2.f;
    m_fDtctRng = 9.f;
    m_fAtkRng = 1.5f;
    m_bInit = false;
    m_bHit = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 2.f;
    m_fRunTimer = 0.f;
    m_szName = L"Yeti";

    m_pTransformCom->Set_Pos(0.f, 2.f, 0.f);
    m_vPrePos = m_pTransformCom->Get_Pos();

    CGameObject::Push_CollisionMgr(OT_MONSTER,0.5);

    m_eState = MONSTER_IDLE;
    m_ePreState = MONSTER_END;

    return S_OK;
}

HRESULT CYeti::Ready_GameObject(D3DXVECTOR3 vPos)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iMaxHealth = m_iCurHealth = 100;
    m_iDamage = 5;
    m_fSpeed = 2.f;
    m_fInitSpeed = 2.f;
    m_fDtctRng = 5.f;
    m_fAtkRng = 1.5f;
    m_bInit = false;
    m_bHit = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 2.f;
    m_fRunTimer = 0.f;
    m_szName = L"Yeti";


    m_pTransformCom->Set_Pos(vPos);
    m_vPrePos = vPos;

    m_fRadius = 0.5f;

    m_lowerBound = { vPos.x - m_fRadius, vPos.y - m_fRadius ,vPos.z - m_fRadius };
    m_upperBound = { vPos.x + m_fRadius, vPos.y + m_fRadius ,vPos.z + m_fRadius };

    m_eObjType = OT_MONSTER;

    m_bColliderIsNone = true;

    m_eState = MONSTER_IDLE;
    m_ePreState = MONSTER_END;

    return S_OK;
}

Engine::_int CYeti::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);


    m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

    if (m_bHit)
    {
        Yeti_Take_Dmg(fTimeDelta);
    }

    ////버그 있으면 수정요청 바람..
    //if (m_fFrame > _float(m_iMaxFrame) && m_eState == MONSTER_ATK)
    //{
    //    m_bAtk = true;


    //    m_fFrame = 0.f;

    //}
    /*else */
    
    //if ((m_fFrame > 5.f || m_fFrame > 13.f) && m_eState == MONSTER_ATK)
    //{
    //    m_bAtk = true;
    //}

    if (m_fFrame > _float(m_iMaxFrame) && m_eState != MONSTER_DEAD)
        m_fFrame = 0.f;


  

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
            wstring szSoundFile;
            if (5 > rand() % 10)
            {
                szSoundFile = L"yeti_detect_1.wav";
            }
            else
            {
                szSoundFile = L"yeti_detect_2.wav";
            }
            Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());
            m_eState = MONSTER_RUN;
        }
    }
    break;
    case MONSTER_RUN:
    {
        if (m_fAtkRng > DistanceToPlayer(m_pPlayer))
        {
            m_bAtk = false;
            m_bAtk2 = false;
            m_eState = MONSTER_ATK;
        }
        else
        {
            m_fRunTimer += fTimeDelta;
            m_fSpeed = m_fInitSpeed * m_fRunTimer + (0.5f * m_fAccel * m_fRunTimer * m_fRunTimer);
            m_fFrameSpeed += 0.1f;
            min(m_fSpeed, m_fMaxSpeed);

            Chase_Player(m_pPlayer, fTimeDelta);
        }
    }
    break;
    case MONSTER_ATK:
    {
        if (m_fAtkRng < DistanceToPlayer(m_pPlayer))
        {  
            m_eState = MONSTER_RUN;
        }
        else
        {
            if (5.f <= m_fFrame && !m_bAtk)  // 5프레임이 넘었고, 아직 데미지를 주지 않았을 때
            {
                wstring szSoundFile;
                if (5 > rand() % 10)
                {
                    szSoundFile = L"yeti_attack_1.wav";
                }
                else
                {
                    szSoundFile = L"yeti_attack_2.wav";
                }
                Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());
                m_pPlayer->Add_Health(-7);
                m_bAtk = true; 
            }
            else if (13.f <= m_fFrame && !m_bAtk2)  // 13프레임이 넘었고, 아직 데미지를 주지 않았을 때
            {
                wstring szSoundFile;
                if (5 > rand() % 10)
                {
                    szSoundFile = L"yeti_attack_1.wav";
                }
                else
                {
                    szSoundFile = L"yeti_attack_2.wav";
                }
                Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());
                m_pPlayer->Add_Health(-7);
                m_bAtk2 = true;  
            }

 
        }
    }
    break;
    case MONSTER_HIT:
    {
        m_fStiffTimer += 10 * fTimeDelta;
        if (m_fStiffSecond < m_fStiffTimer)
        {
            m_fStiffTimer = 0.f;
            m_eState = MONSTER_RUN;
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

void CYeti::LateUpdate_GameObject(const float& fTimeDelta)
{
    Motion_Change();
    __super::LateUpdate_GameObject(fTimeDelta);

}

void CYeti::Render_GameObject()
{
    if (m_eState == MONSTER_DEAD && m_iMaxFrame <= (_uint)(m_fFrame))
    {
        return;
    }

    _matrix   matWorld, matView, matBill;

    m_pTransformCom->Get_WorldMatrix(&matWorld);
    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;

    D3DXMatrixInverse(&matBill, NULL, &matBill);

    m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


void CYeti::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    //CGameObject::Action_On_Collision(pGameObject);
    OBJ_TYPE ot = pGameObject->Get_ObjType();

    switch (ot)
    {
    case OT_PLAYER_BULLET:
    {
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
            pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.08f, 50, 0);
            Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Firework", pParticleObject);
            Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"FlamerEffect", pEffect);
        }
    }
    break;
    case OT_MONSTER:
    {
        CGameObject::Action_On_Collision(pGameObject, fTimeDelta);
        ////몬스터끼리 충돌
        //_float d1, d2;
        //_vec3 vSrc, vDst, vDir;

        //vSrc = Get_TransformCom_Pointer()->Get_Pos();
        //vDst = pGameObject->Get_TransformCom_Pointer()->Get_Pos();

        //d1 = D3DXVec3Length(&(vSrc - vDst));
        //d2 = m_fRadius + pGameObject->Get_Radius();

        ////두 원의 반지름 더한 값이 위치값 뺀거보다 큼
        ////== 충돌했다
        //// 어떤 위치를 바꿀건가 : 침범한 쪽
        //// 어떤 방향으로 미는가 : 당한쪽에서 침범한쪽 == pGO - this

        //if (d1 < d2)
        //{
        //   d2 = d2 - d1;
        //   /*D3DXVec3Normalize(&vDir, &(vDst - vSrc));
        //   vDir = vDir * d2;
        //   dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(vDir + vDst);
        //   */
        //   D3DXVec3Normalize(&vDir, &(vSrc - vDst));
        //   vDir = vDir * d2;
        //   m_pTransformCom->Set_Pos(vSrc + vDir);

        //   /*if (99 <= m_tFramePos.size())
        //      dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(*m_tFramePos[1]);*/

        //}
    }

    case OT_FLOOR:
    case OT_CEILING:
    case OT_WALL:
    {
    }
    break;
    }

    __super::Action_On_Collision(pGameObject, fTimeDelta);

    m_pTransformCom->Set_Y(2.f);
}

void CYeti::Action_On_Collision(vector<CLine*> pvecLines)
{
    if (m_eState == MONSTER_RUN)
    {
        m_fFrameSpeed = 10.f;
        m_fRunTimer = m_fInitSpeed;
    }
    CGameObject::Action_On_Collision(pvecLines);
}

HRESULT CYeti::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    //Textures
    pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_YetiIdleTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_YetiIdleTexture", pComponent });

    pComponent = m_pTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_YetiRunTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_YetiRunTexture", pComponent });

    pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_YetiAtkTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_YetiAtkTexture", pComponent });

    pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_YetiHitTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_YetiHitTexture", pComponent });

    pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_YetiDeadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_YetiDeadTexture", pComponent });
    //Texture Ends

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

CYeti* CYeti::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CYeti* pInstance = DBG_NEW CYeti(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

CYeti* CYeti::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos)
{
    CYeti* pInstance = DBG_NEW CYeti(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject(vPos)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CYeti::Free()
{
    CMonster::Free();
}

void CYeti::Motion_Change()
{
    if (m_eState != m_ePreState)
    {
        m_fRunTimer = 0.f;

        switch (m_eState)
        {
        case MONSTER_IDLE:
        {
            m_pTextureCom = m_pTexCom_Idle;
            m_iMaxFrame = 1;
            m_fFrameSpeed = 1.f;
        }
        break;
        case MONSTER_RUN:
        {
            m_pTextureCom = m_pTexCom_Run;
            m_iMaxFrame = 8;
            m_fFrameSpeed = 10.f;
        }
        break;
        case MONSTER_ATK:
        {
            m_pTextureCom = m_pTexCom_Atk;
            m_iMaxFrame = 17;
            m_fFrameSpeed = 10.f;
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
            m_iMaxFrame = 13;
            m_fFrameSpeed = 10.f;
        }
        break;
        }

        m_ePreState = m_eState;

    }
}

void CYeti::Yeti_Take_Dmg(const _float& fTimeDelta)
{
    wstring szSoundFile = L"";

    if (m_eState != MONSTER_DEAD)
    {
        if (0 < m_iCurHealth)
        {
            m_eState = MONSTER_HIT;
            Knock_Back(m_pPlayer, m_fSpeed * 0.8f, fTimeDelta);

            //사운드 추가
            if (5 > rand() % 10)
            {
                szSoundFile = L"yeti_pain_1.wav";
            }
            else
            {
                szSoundFile = L"yeti_pain_2.wav";
            }
        }
        else if (0 >= m_iCurHealth)
        {
            m_eState = MONSTER_DEAD;
            m_fFrame = 0.f;
            szSoundFile = L"yeti_death.wav";

            //피격 이펙트 출력
            CGameObject* pGameObject;
            _vec3 vPos;
            m_pTransformCom->Get_Info(INFO_POS, &vPos);
            pGameObject = CEffect::Create(m_pGraphicDev, FX_OVERKILL, vPos);

            if (pGameObject != nullptr)
            {
                //이펙트 플레이어 방향으로 조금 이동
                _vec3 vDir;
                D3DXVec3Normalize(&vDir, &(m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vPos));
                CTransform* transf = dynamic_cast<CTransform*>(pGameObject->Get_TransformCom_Pointer());
                transf->Set_Pos((vPos + vDir * 0.1f));
                //이펙트 크기 조정
                transf->Set_Scale({ 0.8f, 0.8f, 0.8f });

                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"YetiDeathEffect", pGameObject);
                CGameObject* pParticleObject;
                _vec3 vPos = m_pTransformCom->Get_Pos();
                pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.1f, 4, 0);
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Firework", pParticleObject);
                pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.2f, 2, 1);
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Gib", pParticleObject);
            }


        }
        m_bHit = false;
        Engine::Get_SoundMgr()->Play_Sound(szSoundFile, MONSTERSOUND, 0.56f);
        //Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());

    }
}