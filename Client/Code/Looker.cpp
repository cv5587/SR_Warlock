#include "stdafx.h"
#include "..\Header\Looker.h"
#include "MonsterBullet.h"

CLooker::CLooker(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev)
{
}

CLooker::CLooker(const CLooker& rhs)
    : CMonster(rhs)
{

}

CLooker::~CLooker()
{
}

HRESULT CLooker::Ready_GameObject()
{

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iMaxHealth = m_iCurHealth = 100;
    m_iDamage = 5;
    m_fSpeed = 1.f;
    m_fDtctRng = 12.f;
    m_fAtkRng = 10.f;
    m_bInit = false;
    m_bHit = false;
    m_bAtk = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 5.f;
    m_szName = L"Looker";

    //  m_pTransformCom->Set_Pos(5.f, 1.f, 5.f);
    m_vPrePos = m_pTransformCom->Get_Pos();

    m_eState = MONSTER_IDLE;
    m_ePreState = MONSTER_END;

    //test
    m_eObjType = OT_MONSTER;
    m_fRadius = 0.3f;


    CGameObject::Push_CollisionMgr(m_eObjType, m_fRadius);

    return S_OK;
}

HRESULT CLooker::Ready_GameObject(D3DXVECTOR3 vPos)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iMaxHealth = m_iCurHealth = 100;
    m_iDamage = 5;
    m_fSpeed = 1.f;
    m_fDtctRng = 12.f;
    m_fAtkRng = 10.f;
    m_bInit = false;
    m_bHit = false;
    m_bAtk = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 5.f;
    m_szName = L"Looker";

    m_pTransformCom->Set_Pos(vPos);
    m_vPrePos = vPos;

    m_eState = MONSTER_IDLE;
    m_ePreState = MONSTER_END;

    m_eObjType = OT_MONSTER;
    m_fRadius = 0.4f;

    m_lowerBound = { vPos.x - m_fRadius, vPos.y - m_fRadius ,vPos.z - m_fRadius };
    m_upperBound = { vPos.x + m_fRadius, vPos.y + m_fRadius ,vPos.z + m_fRadius };

    m_bColliderIsNone = true;



    return S_OK;
}

Engine::_int CLooker::Update_GameObject(const _float& fTimeDelta)
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
        Looker_Take_Dmg(fTimeDelta);
    }

    switch (m_eState)
    {
    case MONSTER_IDLE:
    {
        if (!m_bInit)
        {
            m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
            m_bInit = true;
            //test
            m_fDtctRng = 30.f;
            m_fAtkRng = 30.f;
        }

        if (m_fDtctRng > DistanceToPlayer(m_pPlayer))
        {
            m_eState = MONSTER_RUN;
        }
    }
    break;
    case MONSTER_RUN:
    {
        if (m_fAtkRng > DistanceToPlayer(m_pPlayer))
        {
            wstring szSoundFile;
            if (5 > rand() % 10)
            {
                szSoundFile = L"looker_detect_1.wav";
            }
            else
            {
                szSoundFile = L"looker_detect_2.wav";
            }
            Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());
            m_eState = MONSTER_ATK;

        }
        else
        {
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

                Engine::Get_SoundMgr()->Play_Sound(L"looker_attack.wav", 0.5f, m_pTransformCom->Get_Pos());
                m_bAtk = false;
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

    //TODO ; 몬스터 불렛 리스트 업데이트

    return 0;
}

void CLooker::LateUpdate_GameObject(const float& fTimeDelta)
{
    Motion_Change();
    __super::LateUpdate_GameObject(fTimeDelta);
}

void CLooker::Render_GameObject()
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

void CLooker::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{

    OBJ_TYPE ot = pGameObject->Get_ObjType();

    switch (ot)
    {
    case OT_PLAYER_BULLET:
    {
        Take_Dmg(dynamic_cast<CPlayerBullet*>(pGameObject)->Get_Dmg());
        //피격 이펙트 출력
        CGameObject* pEffect;
        _vec3 vPos = pGameObject->Get_TransformCom_Pointer()->Get_Pos();
        pEffect = CEffect::Create(m_pGraphicDev, FX_BLOOD_YELLOW, vPos);

        if (pEffect != nullptr)
        {
            //이펙트 플레이어 방향으로 조금 이동
            _vec3 vDir;
            D3DXVec3Normalize(&vDir, &(m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vPos));
            CTransform* transf = dynamic_cast<CTransform*>(pEffect->Get_TransformCom_Pointer());
            transf->Set_Pos((vPos + vDir * 0.1f));
            //이펙트 크기 조정
            transf->Set_Scale({ 0.45f, 0.45f, 0.45f });
            CGameObject* pParticleObject;
            pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.08f, 3, 6);
            Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Firework", pParticleObject);
            Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"LookerEffect", pEffect);
        }
    }
    break;
    case OT_MONSTER:
    {
        CGameObject::Action_On_Collision(pGameObject, fTimeDelta);
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

void CLooker::Action_On_Collision(vector<CLine*> pvecLines)
{
    CGameObject::Action_On_Collision(pvecLines);
}

HRESULT CLooker::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    //Textures
    pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerIdleTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_LookerIdleTexture", pComponent });

    pComponent = m_pTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerRunTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_LookerRunTexture", pComponent });

    pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerAtkTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_LookerAtkTexture", pComponent });

    pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerHitTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_LookerHitTexture", pComponent });

    pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerDeadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_LookerDeadTexture", pComponent });
    //Texture Ends

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

CLooker* CLooker::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLooker* pInstance = DBG_NEW CLooker(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

CLooker* CLooker::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3 vPos)
{
    CLooker* pInstance = DBG_NEW CLooker(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject(vPos)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CLooker::Free()
{
    CMonster::Free();
}

void CLooker::Motion_Change()
{
    if (m_eState != m_ePreState)
    {
        switch (m_eState)
        {
        case MONSTER_IDLE:
        {
            m_pTextureCom = m_pTexCom_Idle;
            m_iMaxFrame = 10;
            m_fFrameSpeed = 10.f;
        }
        break;
        case MONSTER_RUN:
        {
            m_pTextureCom = m_pTexCom_Run;
            m_iMaxFrame = 10;
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
            m_iMaxFrame = 15;
            m_fFrameSpeed = 10.f;
        }
        break;
        }

        m_ePreState = m_eState;

    }

}

void CLooker::Looker_Take_Dmg(const _float& fTimeDelta)
{
    wstring szSoundFile = L"";

    if (m_eState != MONSTER_DEAD)
    {
        if (0 < m_iCurHealth)
        {
            m_eState = MONSTER_HIT;
            Knock_Back(m_pPlayer, m_fSpeed * 0.8f, fTimeDelta);
            if (5 > rand() % 10)
            {
                szSoundFile = L"looker_pain_1.wav";
            }
            else
            {
                szSoundFile = L"looker_pain_2.wav";
            }
        }
        else if (0 >= m_iCurHealth)
        {
            m_eState = MONSTER_DEAD;
            m_fFrame = 0.f;
            if (5 > rand() % 10)
            {
                szSoundFile = L"looker_death_1.wav";
            }
            else
            {
                szSoundFile = L"looker_death_2.wav";
            }

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

                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"LookerDeathEffect", pGameObject);
                CGameObject* pParticleObject;
                _vec3 vPos = m_pTransformCom->Get_Pos();
                pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.1f, 4, 0);
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Firework", pParticleObject);
                pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.2f, 1, 3);
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Gib", pParticleObject);
            }
        }
        m_bHit = false;
        Engine::Get_SoundMgr()->Play_Sound(szSoundFile, MONSTERSOUND, 0.56f);
        //Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());
    }
}