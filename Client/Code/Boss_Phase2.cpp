#include "stdafx.h"
#include "..\Header\Boss_Phase2.h"
#include "MonsterBullet.h"

#include "Shambler.h"
#include "Looker.h"
#include "Yeti.h"
#include "IceBullet.h"
#include "Boss_Tentacle.h"

CBoss_Phase2::CBoss_Phase2(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev), m_iBulletCount(0), m_bWarp(false), m_bSuperArmor(true), m_iWarpPhase(0),
    m_iNoRandom(0), m_bActive(false), m_bElevate(true), m_fPatTimer(0.f), m_iSummonCnt(0), m_bIsSoundStarted(false)
{
}

CBoss_Phase2::CBoss_Phase2(const CBoss_Phase2& rhs)
    : CMonster(rhs), m_iBulletCount(rhs.m_iBulletCount), m_bWarp(rhs.m_bWarp), m_bSuperArmor(rhs.m_bSuperArmor), m_iWarpPhase(rhs.m_iWarpPhase),
    m_iNoRandom(rhs.m_iNoRandom), m_bActive(rhs.m_bActive), m_bElevate(rhs.m_bActive), m_fPatTimer(rhs.m_fPatTimer), m_iSummonCnt(rhs.m_iSummonCnt)
{
    Ready_GameObject();
}

CBoss_Phase2::~CBoss_Phase2()
{
}

HRESULT CBoss_Phase2::Ready_GameObject()
{

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iMaxHealth = m_iCurHealth = 300;
    m_iDamage = 5;
    m_fSpeed = 0.5f;
    m_fDtctRng = 50.f;
    m_fAtkRng = 15.f;
    m_bInit = true;
    m_bHit = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 3.f;
    m_fPatTimer = 0.f;
    m_iSummonCnt = 0;
    m_pTransformCom->Set_Pos(-4.f, -4.f, 30.f);
    m_pTransformCom->Set_Scale({ 5.f, 5.f, 5.f });
    m_eBoss2State = BOSS2_INACTIVE;
    m_ePreBoss2State = BOSS2_END;
    m_fRadius = 2.f;
    m_szName = L"REAL Cthulhu";

    m_vPrePos = m_pTransformCom->Get_Pos();
    CGameObject::Push_CollisionMgr(OT_MONSTER, m_fRadius);

    return S_OK;
}

Engine::_int CBoss_Phase2::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    if (m_bActive)
    {
        _vec3 vSrc = m_pTransformCom->Get_Pos();

        if (m_bElevate)
        {
            if (!m_bIsSoundStarted)
            {
                m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
                Engine::Get_SoundMgr()->Play_Sound(L"earthquake.wav", EARTHQUAKESOUND, 0.5f);
                m_pPlayer->Set_Camera_Shaking();
                m_bIsSoundStarted = true;
            }

            Elevate(fTimeDelta);
        }


        Engine::Add_RenderGroup(RENDER_ALPHATEST, this);


        m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

        //버그 있으면 수정요청 바람..
        if (m_fFrame > m_iMaxFrame && m_eBoss2State != BOSS2_DEAD)
            m_fFrame = 0.f;


        if (m_bHit)
        {
            Boss_Phase2_Take_Dmg(fTimeDelta);
        }

        switch (m_eBoss2State)
        {
        case BOSS2_INACTIVE:
        {
            if (m_bInit)
            {
                m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

                m_bInit = false;
            }


        }
        break;
        case BOSS2_IDLE:
        {

            m_fPatTimer += 5.f * fTimeDelta;
            if (5.f < m_fPatTimer)   //Pattern Cool Time
            {
                m_fPatTimer = 0.f;
                if (m_fDtctRng > DistanceToPlayer(m_pPlayer))
                {
                    m_fAtkTimer = 0.f;
                    //m_iNoRandom = 0/*rand() % 3*/;
                    //m_iNoRandom = 3;
                    switch (m_iNoRandom)
                    {
                    case 0:
                        m_eBoss2State = BOSS2_ATK;
                        m_iNoRandom++;
                        break;
                    case 1:
                        m_eBoss2State = BOSS2_MAGIC;
                        m_iNoRandom++;
                        break;
                    case 2:
                    {
                        if (m_iCurHealth < 150)
                        {
                            m_eBoss2State = BOSS2_SUMMON;
                        }
                        else
                        {
                            m_eBoss2State = BOSS2_ATK;
                        }
                        m_iNoRandom++;
                    }
                    break;
                    case 3:
                    {
                        m_eBoss2State = BOSS2_ICE;
                        m_iNoRandom++;
                    }
                    break;
                    default:
                    {
                        m_iNoRandom = 0;
                    }
                    break;
                    }
                }
            }

        }
        break;
        case BOSS2_HIT:
        {
            m_fStiffTimer += fTimeDelta;
            if (m_fStiffTimer > m_fStiffSecond)
            {
                m_fStiffTimer = 0.f;
                m_eBoss2State = m_ePreBoss2State;
            }
        }
        break;
        case BOSS2_ATK:
        {

            _vec3 vSrc, vDst;
            vSrc = m_pTransformCom->Get_Pos();
            vDst = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();

            CGameObject* pGameObject;

            pGameObject = CMonsterBullet::Create(m_pGraphicDev, BULLET_SLOW, vSrc, vDst);

            if (pGameObject != nullptr)
            {
                CTransform* transf = dynamic_cast<CTransform*>(pGameObject->Get_TransformCom_Pointer());
                transf->Set_Scale({ 0.6f, 0.6f, 0.6f });

                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Boss2Bullet", pGameObject);
            }

            dynamic_cast<CMonsterBullet*>(pGameObject)->Set_Tracker();
            dynamic_cast<CMonsterBullet*>(pGameObject)->Set_Speed(1.f);
            m_eBoss2State = BOSS2_IDLE;

        }
        break;
        case BOSS2_MAGIC:
        {
            _float d = DistanceToPlayer(m_pPlayer);

            if (m_fAtkRng < d)
            {
                m_eBoss2State = BOSS2_IDLE;
            }
            else
            {
                m_fAtkTimer += fTimeDelta;
                _vec3 vFrom, vTo;
                m_pTransformCom->Get_Info(INFO_POS, &vFrom);
                vTo = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();

                if (0.5f < m_fAtkTimer && m_iBulletCount == 0)
                {
                    Boss_SlowBullet(vFrom, vTo, -15.f);
                    m_iBulletCount++;
                }
                else if (1.f < m_fAtkTimer && m_iBulletCount == 1)
                {
                    Boss_SlowBullet(vFrom, vTo, -10.f);
                    m_iBulletCount++;
                }
                else if (1.5f < m_fAtkTimer && m_iBulletCount == 2)
                {
                    Boss_SlowBullet(vFrom, vTo, -5.f);
                    m_iBulletCount++;
                }
                else if (2.f < m_fAtkTimer && m_iBulletCount == 3)
                {
                    Boss_SlowBullet(vFrom, vTo, 0.f);
                    m_iBulletCount++;
                }
                else if (2.5f < m_fAtkTimer && m_iBulletCount == 4)
                {
                    Boss_SlowBullet(vFrom, vTo, 5.f);
                    m_iBulletCount++;
                }
                else if (3.0f < m_fAtkTimer && m_iBulletCount == 5)
                {
                    Boss_SlowBullet(vFrom, vTo, 10.f);
                    m_iBulletCount++;

                }
                else if (3.5f < m_fAtkTimer && m_iBulletCount == 6)
                {
                    Boss_SlowBullet(vFrom, vTo, 15.f);
                    m_fAtkTimer = 0.f;
                    m_iBulletCount = 0;
                    m_eBoss2State = BOSS2_IDLE;
                }
            }
        }
        break;
        case BOSS2_SUMMON:
        {
            m_iSummonCnt++;

            if (2 < m_iSummonCnt) //서먼카운트 3 되면 소환
            {
                if (m_lTentacles.empty())
                {
                    for (int i = 0; i < 5; ++i)
                    {
                        m_lTentacles.push_back(CBoss_Tentacle::Create(m_pGraphicDev));



                        if (m_lTentacles.back() != nullptr)
                        {
                            Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BossTentacleSummon", m_lTentacles.back());
                        }
                        float radius = 11.0f;

                        float angle = D3DXToRadian(float(72 * i));
                        _vec3 point;


                        point.x = vSrc.x + radius * cosf(angle);
                        point.y = 2.1f;
                        point.z = vSrc.z + radius * sinf(angle);

                        CTransform* trns = dynamic_cast<CTransform*>(m_lTentacles.back()->Get_Component(ID_DYNAMIC, L"Proto_Transform"));
                        trns->Set_Pos(point);
                    }

                    m_bSuperArmor = true;
                }
                m_iSummonCnt = 0;
            }

            m_eBoss2State = BOSS2_IDLE;

        }
        break;
        case BOSS2_ICE:
        {
            _float d = DistanceToPlayer(m_pPlayer);

            if (m_fAtkRng < d)
            {
                m_eBoss2State = BOSS2_IDLE;
            }
            else
            {
                wstring szSoundFile;

                m_fAtkTimer += 10 * fTimeDelta;
                _vec3 vFrom, vTo, vDst;
                vFrom = m_pTransformCom->Get_Pos();
                D3DXVec3Normalize(&vTo, &(m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vFrom));
                vDst = vFrom + vTo * d;
                if (5.f < m_fAtkTimer && m_iBulletCount == 0)
                {
                    int i = rand() % 15;
                    if (5 > i)
                    {
                        szSoundFile = L"fleshy_stab_1.wav";
                    }
                    else
                    {
                        szSoundFile = L"fleshy_stab_2.wav";
                    }
                    Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());

                    m_lSpikes.push_back(CIceBullet::Create(m_pGraphicDev, rand() % 3 + 4, { vDst.x * 0.1f /*+ _float(rand()%3 - 2)*/, 2.1f, vDst.z * 0.1f /*+ _float(rand() % 3 - 2)*/ }));
                    m_iBulletCount++;
                }
                else if (10.f < m_fAtkTimer && m_iBulletCount == 1)
                {
                    int i = rand() % 15;
                    if (5 > i)
                    {
                        szSoundFile = L"fleshy_stab_1.wav";
                    }
                    else
                    {
                        szSoundFile = L"fleshy_stab_2.wav";
                    }
                    Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());

                    m_lSpikes.push_back(CIceBullet::Create(m_pGraphicDev, rand() % 3 + 4, { vDst.x * 0.3f /*+ _float(rand() % 3 - 2)*/, 2.1f, vDst.z * 0.3f /*+ _float(rand() % 3 - 2)*/ }));
                    m_iBulletCount++;
                }
                else if (15.f < m_fAtkTimer && m_iBulletCount == 2)
                {
                    int i = rand() % 15;
                    if (5 > i)
                    {
                        szSoundFile = L"fleshy_stab_1.wav";
                    }
                    else
                    {
                        szSoundFile = L"fleshy_stab_2.wav";
                    }
                    Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());

                    m_lSpikes.push_back(CIceBullet::Create(m_pGraphicDev, rand() % 3 + 4, { vDst.x * 0.5f /*+ _float(rand() % 3 - 2)*/, 2.1f, vDst.z * 0.5f /*+ _float(rand() % 3 - 2) */ }));
                    m_iBulletCount++;
                }
                else if (20.f < m_fAtkTimer && m_iBulletCount == 3)
                {
                    int i = rand() % 15;
                    if (5 > i)
                    {
                        szSoundFile = L"fleshy_stab_1.wav";
                    }
                    else
                    {
                        szSoundFile = L"fleshy_stab_2.wav";
                    }
                    Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());

                    m_lSpikes.push_back(CIceBullet::Create(m_pGraphicDev, rand() % 3 + 4, { vDst.x * 0.7f /*+ _float(rand() % 3 - 2)*/, 2.1f, vDst.z * 0.7f /*+ _float(rand() % 3 - 2) */ }));
                    m_iBulletCount++;
                }
                else if (25.f < m_fAtkTimer && m_iBulletCount == 4)
                {
                    int i = rand() % 15;
                    if (5 > i)
                    {
                        szSoundFile = L"fleshy_stab_1.wav";
                    }
                    else
                    {
                        szSoundFile = L"fleshy_stab_2.wav";
                    }
                    Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 0.8f, m_pTransformCom->Get_Pos());

                    m_lSpikes.push_back(CIceBullet::Create(m_pGraphicDev, rand() % 3 + 4, { vDst.x * 0.9f /*+ _float(rand() % 3 - 2)*/, 2.1f, vDst.z /** 0.9f + _float(rand() % 3 - 2)*/ }));
                    m_fAtkTimer = 0.f;
                    m_iBulletCount = 0;
                    m_bAtk = false;
                    m_eBoss2State = BOSS2_IDLE;
                }

            }
        }
        break;
        case BOSS2_DEAD:
        {
            if (m_iMaxFrame <= (_uint)(m_fFrame))
            {
               // return OBJ_DEAD;
                m_fFrame = _float(m_iMaxFrame - 1);
            }

        }
        break;

        }
    }


    if (!m_lSpikes.empty())
    {
        for (auto iter : m_lSpikes)
        {
            iter->Update_GameObject(fTimeDelta);
        }
    }

    return OBJ_NOEVENT;
}

void CBoss_Phase2::LateUpdate_GameObject(const float& fTimeDelta)
{
    if (m_bActive)
    {
        Motion_Change();
        __super::LateUpdate_GameObject(fTimeDelta);

        if (!m_lTentacles.empty())
        {
            for (auto iter = m_lTentacles.begin(); iter != m_lTentacles.end();)
            {
                if ((*iter)->Get_Dead())
                {
                    iter = m_lTentacles.erase(iter);
                }
                else
                {
                    iter++;
                }
            }
        }
        else if (m_lTentacles.empty() && m_bInit && m_bSuperArmor)
        {
            m_bSuperArmor = false;
            m_iMaxHealth = m_iCurHealth = 300;
        }

        if (!m_lSpikes.empty())
        {
            for (auto iter = m_lSpikes.begin(); iter != m_lSpikes.end();)
            {
                if ((*iter)->Get_Dead())
                {
                    Safe_Release(*iter);
                    iter = m_lSpikes.erase(iter);
                }
                else
                {
                    (*iter)->LateUpdate_GameObject(fTimeDelta);
                    iter++;
                }
            }
        }
    }

    Compute_ViewZ(m_pTransformCom->Get_Pos());
}

void CBoss_Phase2::Render_GameObject()
{
    if (!m_bActive) return;

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
    m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CBoss_Phase2::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{

    if (pGameObject->Get_ObjType() == OT_PLAYER_BULLET)
    {
        if (!m_bSuperArmor)
        {
            Take_Dmg(dynamic_cast<CPlayerBullet*>(pGameObject)->Get_Dmg());

            CGameObject* pEffect;
            _vec3 vPos = m_pTransformCom->Get_Pos();
            pEffect = CEffect::Create(m_pGraphicDev, FX_BLOOD_BLACK, vPos);

            if (pEffect != nullptr)
            {
                //이펙트 플레이어 방향으로 조금 이동
                _vec3 vDir;
                D3DXVec3Normalize(&vDir, &(m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vPos));
                CTransform* transf = dynamic_cast<CTransform*>(pEffect->Get_TransformCom_Pointer());
                transf->Set_Pos((vPos + vDir * 0.1f));
                //이펙트 크기 조정
                transf->Set_Scale({ 0.8f, 0.8f, 0.8f });

                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BossEffect", pEffect);
            }
        }
        

    }
    __super::Action_On_Collision(pGameObject, fTimeDelta);
}

void CBoss_Phase2::Action_On_Collision(vector<CLine*> pvecLines)
{
}


void CBoss_Phase2::Elevate(const _float& fTimeDelta)
{

    _vec3 yUp = { 0.f, 1.f, 0.f };
    Move_Pos(yUp, m_fSpeed, fTimeDelta);

    _vec3 pos = m_pTransformCom->Get_Pos();

    Engine::Get_SoundMgr()->Play_Sound(L"Cthulhu_active.wav", BOSSSOUND, 0.9f, false);

    if (6.f < pos.y)    
    {
        Engine::Get_SoundMgr()->Play_BGM(L"BGM_BossStage2.wav", 0.25f);

        _vec3 vSrc = m_pTransformCom->Get_Pos();

        for (int i = 0; i < 10; ++i)
        {
            m_lTentacles.push_back(CBoss_Tentacle::Create(m_pGraphicDev));

            if (m_lTentacles.back() != nullptr)
            {
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BossTentacleSummon", m_lTentacles.back());
            }
            float radius = 10.0f;

            float angle = D3DXToRadian(float(36 * i));
            _vec3 point;


            point.x = vSrc.x + radius * cosf(angle);
            point.y = 2.1f;
            point.z = vSrc.z + radius * sinf(angle);

            CTransform* trns = dynamic_cast<CTransform*>(m_lTentacles.back()->Get_Component(ID_DYNAMIC, L"Proto_Transform"));
            trns->Set_Pos(point);
        }


        m_bElevate = false;
        m_eBoss2State = BOSS2_IDLE;
        m_bInit = true;
    }

}

HRESULT CBoss_Phase2::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    //Textures
    pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Phase2_IdleTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Phase2_IdleTexture", pComponent });

    pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Phase2_AtkTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Phase2_AtkTexture", pComponent });

    pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Phase2_DeadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Phase2_DeadTexture", pComponent });

    //Texture Ends

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

CBoss_Phase2* CBoss_Phase2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBoss_Phase2* pInstance = DBG_NEW CBoss_Phase2(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CBoss_Phase2::Free()
{
    CMonster::Free();
    for_each(m_lTentacles.begin(), m_lTentacles.end(), CDeleteObj());
    for_each(m_lSpikes.begin(), m_lSpikes.end(), CDeleteObj());
}

void CBoss_Phase2::Motion_Change()
{
    if (m_eBoss2State != m_ePreBoss2State)
    {
        switch (m_eBoss2State)
        {
        case BOSS2_INACTIVE:
        case BOSS2_IDLE:
        case BOSS2_HIT:
        {
            m_pTextureCom = m_pTexCom_Idle;
            m_iMaxFrame = 7;
            m_fFrameSpeed = 1.f;
        }
        break;
        case BOSS2_ATK:
        case BOSS2_WARP:
        case BOSS2_MAGIC:
        case BOSS2_SUMMON:
        case BOSS2_ICE:
        {
            m_pTextureCom = m_pTexCom_Atk;
            m_iMaxFrame = 4;
            m_fFrameSpeed = 5.f;
        }
        break;
        case BOSS2_DEAD:
        {
            CGameObject::Remove_Collider();
            m_bSuperArmor = true;
            m_pTextureCom = m_pTexCom_Dead;
            m_iMaxFrame = 8;
            m_fFrameSpeed = 7.f;
        }
        break;
        }

        if (m_eBoss2State != BOSS2_HIT)
            m_ePreBoss2State = m_eBoss2State;
    }
}

void CBoss_Phase2::Boss_Phase2_Take_Dmg(const _float& fTimeDelta)
{
    if (!m_bSuperArmor)
    {
        wstring szSoundFile = L"";

        if (0 < m_iCurHealth)
        {
            m_eBoss2State = BOSS2_HIT;

            int i = rand() % 15;
            if (5 > i)
            {
                szSoundFile = L"Cthulhu_pain_01.wav";
            }
            else if (10 > i)
            {
                szSoundFile = L"Cthulhu_pain_02.wav";
            }
            else
            {
                szSoundFile = L"Cthulhu_pain_03.wav";
            }
        }
        else if (0 >= m_iCurHealth)
        {
            m_eBoss2State = BOSS2_DEAD;
            m_fFrame = 0.f;
            szSoundFile = L"Cthulhu_death_01.wav";

        }

        m_bHit = false;
        Engine::Get_SoundMgr()->Play_Sound(szSoundFile, MONSTERSOUND, 0.56f);

    }
    else
    {
        //m_iCurHealth = m_iPreHealth;
    }
}

void CBoss_Phase2::Boss_SlowBullet(_vec3 vSrc, _vec3 vDst, _float fAngle)
{
    CGameObject* pGameObject;

    pGameObject = CMonsterBullet::Create(m_pGraphicDev, BULLET_MAGIC, vSrc, vDst);

    if (pGameObject != nullptr)
    {
        CTransform* transf = dynamic_cast<CTransform*>(pGameObject->Get_TransformCom_Pointer());
        transf->Set_Scale({ 0.6f, 0.6f, 0.6f });

        Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Boss2Bullet", pGameObject);
    }

    dynamic_cast<CMonsterBullet*>(pGameObject)->Set_Angle(fAngle);
}