#include "stdafx.h"
#include "..\Header\Boss_Phase1.h"
#include "MonsterBullet.h"

#include "Shambler.h"
#include "Looker.h"
#include "Yeti.h"
#include "Boss_Phase2.h"
CBoss_Phase1::CBoss_Phase1(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev), m_iBulletCount(0), m_bWarp(false), m_bSuperArmor(false), m_iWarpPhase(0), m_iNoRandom(0), m_iPreHealth(300),
    m_bHallucinate(false), m_iHalluPhase(0), m_bHalluDmged(false), m_iRealIndex(0), m_fPatSecond(5.f)
{
}

CBoss_Phase1::CBoss_Phase1(const CBoss_Phase1& rhs)
    : CMonster(rhs), m_iBulletCount(rhs.m_iBulletCount), m_bWarp(rhs.m_bWarp), m_bSuperArmor(rhs.m_bSuperArmor),
    m_iWarpPhase(rhs.m_iWarpPhase), m_iNoRandom(rhs.m_iNoRandom), m_iPreHealth(rhs.m_iPreHealth),
    m_bHallucinate(rhs.m_bHallucinate), m_iHalluPhase(rhs.m_iHalluPhase), m_bHalluDmged(rhs.m_bHalluDmged), m_iRealIndex(0), m_fPatSecond(5.f)
{
    Ready_GameObject();
}

CBoss_Phase1::~CBoss_Phase1()
{
}

HRESULT CBoss_Phase1::Ready_GameObject()
{

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Scale({ 3.f, 3.f, 3.f });
    m_iMaxHealth = m_iCurHealth = m_iPreHealth = 300;
    m_iDamage = 5;
    m_fSpeed = 0.5f;
    m_fDtctRng = 40.f;
    m_fAtkRng = 30.f;
    m_bInit = false;
    m_bHit = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 3.f;
    m_pTransformCom->Set_Pos(-3.f, 10.f, 37.f);
    m_eBoss1State = BOSS1_IDLE;
    m_ePreBoss1State = BOSS1_END;
    m_szName = L"Cthulhu";

    m_vPrePos = m_pTransformCom->Get_Pos();
    CGameObject::Push_CollisionMgr(OT_MONSTER, 2.f);

    return S_OK;
}

Engine::_int CBoss_Phase1::Update_GameObject(const _float& fTimeDelta)
{

    Engine::Add_RenderGroup(RENDER_ALPHA, this);

    CGameObject::Update_GameObject(fTimeDelta);

    m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

    //버그 있으면 수정요청 바람..
    if (m_fFrame > m_iMaxFrame && m_eBoss1State != BOSS1_DEAD)
        m_fFrame = 0.f;


    if (m_bHit)
    {
        Boss_Phase1_Take_Dmg(fTimeDelta);
    }

    switch (m_eBoss1State)
    {
    case BOSS1_IDLE:
    {
        if (!m_bInit)
        {
            m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
            m_bInit = true;
        }

        if (m_fDtctRng > DistanceToPlayer(m_pPlayer))
        {
            //m_iNoRandom = 0/*rand() % 3*/;
            m_fPatTimer += fTimeDelta;

            if (m_fPatTimer < 5.f)
            {
                m_fPatTimer = 0.f;

                switch (m_iNoRandom)
                {
                case 0:
                    m_fAtkTimer = 0.f;
                    m_eBoss1State = BOSS1_ATK;
                    m_iNoRandom++;
                    break;
                case 1:
                    m_fAtkTimer = 0.f;
                    m_eBoss1State = BOSS1_WARP;
                    m_iNoRandom++;
                    break;
                case 2:
                    m_fAtkTimer = 0.f;
                    m_eBoss1State = BOSS1_MAGIC;
                    m_iNoRandom++;
                    break;
                case 3:
                    if (200 > m_iCurHealth)
                    {
                        _vec3 vPos = m_pTransformCom->Get_Pos();
                        m_bSuperArmor = true;
                        m_bHalluDmged = false;
                        m_pTransformCom->Set_Pos({ vPos.x, 10.f, vPos.z });
                        m_eBoss1State = BOSS1_HALLUCINATE;
                    }
                    else
                    {
                        m_eBoss1State = BOSS1_WARP;
                    }
                    m_fAtkTimer = 0.f;
                    m_iNoRandom = 0;
                default:
                    m_iNoRandom = 0;
                    break;
                }
            }
        }
        else
        {
            m_eBoss1State = BOSS1_RUN;
        }
    }
    break;
    case BOSS1_RUN:
    {
        if (m_fAtkRng > DistanceToPlayer(m_pPlayer))
        {
            m_eBoss1State = BOSS1_IDLE;
        }
        else
        {
            Chase_Player(m_pPlayer, fTimeDelta);
        }
    }
    break;
    case BOSS1_ATK:
    {
        _float d = DistanceToPlayer(m_pPlayer);

        if (m_fAtkRng < d)
        {
            m_eBoss1State = BOSS1_RUN;
        }
        else
        {
            m_fAtkTimer += fTimeDelta;
            _vec3 vFrom, vTo;
            vFrom = m_pTransformCom->Get_Pos();
            vTo = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();

            if (0.5f < m_fAtkTimer && m_fAtkTimer <= 1.f && m_iBulletCount == 0)
            {
                Boss_SlowBullet(vFrom, vTo, -30.f);
                m_iBulletCount++;
            }
            else if (1.f < m_fAtkTimer && m_fAtkTimer <= 1.5f && m_iBulletCount == 1)
            {
                Boss_SlowBullet(vFrom, vTo, -15.f);
                m_iBulletCount++;
            }
            else if (1.5f < m_fAtkTimer && m_fAtkTimer <= 2.f && m_iBulletCount == 2)
            {
                Boss_SlowBullet(vFrom, vTo, 0.f);
                m_iBulletCount++;
            }
            else if (2.f < m_fAtkTimer && m_fAtkTimer <= 2.5f && m_iBulletCount == 3)
            {
                Boss_SlowBullet(vFrom, vTo, 15.f);
                m_iBulletCount++;
            }
            else if (2.5f < m_fAtkTimer && m_iBulletCount == 4)
            {
                Boss_SlowBullet(vFrom, vTo, 30.f);
                m_fAtkTimer = 0.f;
                m_iBulletCount = 0;
            }
        }
    }
    break;
    case BOSS1_WARP:
    {
        m_bSuperArmor = true;
        m_fAtkTimer += fTimeDelta;
        _vec3 vPos = m_pTransformCom->Get_Pos();
        switch (m_iWarpPhase)
        {
        case 0:
        {
            if (0 < m_iAlpha && m_fAtkTimer < 10.f)
            {
                m_fAtkTimer = 0.f;
                --m_iAlpha;
            }
            else if (0 >= m_iAlpha)
            {
                ++m_iWarpPhase;
                CYeti* pYeti;
                _vec3 vPos;
                m_pTransformCom->Get_Info(INFO_POS, &vPos);
                pYeti = CYeti::Create(m_pGraphicDev);
                pYeti->Set_Detect_Range(20.f);
                dynamic_cast<CTransform*>(pYeti->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos({ vPos.x, 2.1f, vPos.z });
                if (pYeti != nullptr)
                {
                    Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BossMonsters", pYeti);
                }

            }
        }
        break;
        case 1:
        {
            _float rndX, rndY, rndZ;
            rndX = _float(-rand() % 10);
            rndY = _float(9 + rand() % 7);
            rndZ = _float(25 + rand() % 15);

            m_pTransformCom->Set_Pos({ rndX, rndY, rndZ });
            ++m_iWarpPhase;
        }
        break;
        case 2:
        {
            if (255 > m_iAlpha && m_fAtkTimer < 10.f)
            {
                m_fAtkTimer = 0.f;
                ++m_iAlpha;
            }
            else if (255 <= m_iAlpha)
            {
                ++m_iWarpPhase;
            }
        }
        break;
        default:
        {
            m_iWarpPhase = 0;
            m_fAtkTimer = 0.f;
            m_bSuperArmor = false;
            m_eBoss1State = BOSS1_RUN;

        }
        break;
        }
    }
    break;
    case BOSS1_MAGIC:
    {
        if (m_fAtkRng < DistanceToPlayer(m_pPlayer))
        {
            m_eBoss1State = BOSS1_RUN;
        }
        else
        {
            m_fAtkTimer += 10 * fTimeDelta;
            if (30.f < m_fAtkTimer)
            {
                m_fAtkTimer = 0.f;

                _float d = DistanceToPlayer(m_pPlayer);

                _vec3 vFrom, vTo;
                m_pTransformCom->Get_Info(INFO_POS, &vFrom);
                vTo = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();


                CGameObject* pGameObject;

                pGameObject = CMonsterBullet::Create(m_pGraphicDev, BULLET_MAGIC, vFrom, vTo);

                if (pGameObject != nullptr)
                {
                    CTransform* transf = dynamic_cast<CTransform*>(pGameObject->Get_TransformCom_Pointer());
                    //총알 크기 조정
                    transf->Set_Scale({ 0.6f, 0.6f, 0.6f });

                    Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BossBullet", pGameObject);
                }

                dynamic_cast<CMonsterBullet*>(pGameObject)->Set_Angle(0.f);
            }
        }
    }
    break;
    case BOSS1_HALLUCINATE:
    {
        //  m_bSuperArmor = true;
        m_fAtkTimer += fTimeDelta;
        switch (m_iHalluPhase)
        {
        case 0:
        {
            if (0 < m_iAlpha && m_fAtkTimer < 10.f)
            {
                m_fAtkTimer = 0.f;
                --m_iAlpha;
            }
            else if (0 >= m_iAlpha)
            {
                m_fAtkTimer = 0.f;
                m_bHallucinate = true;

                //충돌박스 랜덤위치로 보내기
                //분신 위치 구하기
                float angleSize = (2.f * D3DX_PI) / 5.f;
                for (int i = 0; i < 5; ++i)
                {
                    float angle = i * angleSize;
                    _vec3 vPos = m_pTransformCom->Get_Pos();
                    vPos.x += cos(angle) * 10.0f;
                    vPos.z += sin(angle) * 10.0f;
                    m_tHalluPoses[i] = vPos;
                }

                //분신 중 랜덤위치로 이동
                m_iRealIndex = rand() % 5;
                m_pTransformCom->Set_Pos(m_tHalluPoses[m_iRealIndex]);
                ++m_iHalluPhase;
            }
        }
        break;
        case 1:
        {
            if (255 > m_iAlpha && m_fAtkTimer < 10.f)
            {
                m_fAtkTimer = 0.f;
                ++m_iAlpha;
            }
            else if (255 <= m_iAlpha)
            {
                m_fAtkTimer = 0.f;
                m_bSuperArmor = false;
                ++m_iHalluPhase;
            }
        }
        break;
        case 2:
        {
            if (m_fAtkTimer > 1.5f)
            {
                //Atk 패턴 추가
                //5개 위치에서
                _vec3 vTo;
                vTo = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();

                CGameObject* pGameObject;

                for (int i = 0; i < 5; ++i)
                {
                    pGameObject = CMonsterBullet::Create(m_pGraphicDev, BULLET_SPIRAL, m_tHalluPoses[i], vTo);

                    if (pGameObject != nullptr)
                    {
                        CTransform* transf = dynamic_cast<CTransform*>(pGameObject->Get_TransformCom_Pointer());
                        transf->Set_Scale({ 0.6f, 0.6f, 0.6f });
                        Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BossBullet", pGameObject);
                    }
                    if (i != m_iRealIndex)
                    {
                        dynamic_cast<CMonsterBullet*>(pGameObject)->Set_Damage(0.f);
                    }
                }

                m_fAtkTimer = 0.f;
            }

            if (m_bHalluDmged)
            {
                m_bHalluDmged = false;
                m_bSuperArmor = true;
                m_fAtkTimer = 0.f;
                ++m_iHalluPhase;
            }
        }
        break;
        case 3:
        {
            if (0 < m_iAlpha && m_fAtkTimer < 10.f)
            {
                m_fAtkTimer = 0.f;
                --m_iAlpha;
            }
            else if (0 >= m_iAlpha)
            {
                m_fAtkTimer = 0.f;
                m_bHallucinate = false;
                ++m_iHalluPhase;
            }

        }
        break;
        case 4:
        {
            if (255 > m_iAlpha && m_fAtkTimer < 10.f)
            {
                m_fAtkTimer = 0.f;
                ++m_iAlpha;
            }
            else if (255 <= m_iAlpha)
            {
                ++m_iHalluPhase;
            }
        }
        break;
        default:
        {
            m_iHalluPhase = 0;
            m_fAtkTimer = 0.f;
            m_bSuperArmor = false;
            m_eBoss1State = BOSS1_RUN;

        }
        break;
        }
    }
    break;
    case BOSS1_HIT:
    {
        m_fStiffTimer += 10 * fTimeDelta;
        if (m_fStiffSecond < m_fStiffTimer)
        {
            m_fStiffTimer = 0.f;
            m_eBoss1State = BOSS1_RUN;
        }
    }
    break;
    case BOSS1_DEAD:
    {
        if (m_iMaxFrame <= (_uint)(m_fFrame))
        {
            CBoss_Phase2* boss = dynamic_cast<CBoss_Phase2*>(Engine::Get_GameObject(L"GameLogic", L"Boss2"));
            boss->Set_Active();
            return OBJ_DEAD;
        }

    }
    break;

    }

    return OBJ_NOEVENT;
}

void CBoss_Phase1::LateUpdate_GameObject(const float& fTimeDelta)
{
    Motion_Change();
    __super::LateUpdate_GameObject(fTimeDelta);

    _vec3   vPos = m_pTransformCom->Get_Pos();
    __super::Compute_ViewZ(vPos);

    //if(m_eBoss1State != BOSS1_WARP)
       //m_vPos = vPos;

    if (m_iCurHealth != m_iPreHealth) m_iPreHealth = m_iCurHealth;
}

void CBoss_Phase1::Render_GameObject()
{
    if (m_eBoss1State == BOSS1_DEAD && m_iMaxFrame <= (_uint)(m_fFrame))
    {
        return;
    }

    if (!m_bHallucinate)
    {
        _vec3 vPos = m_pTransformCom->Get_Pos();
        _matrix   matWorld, matView, matBill;
        m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
        D3DXMatrixInverse(&matView, NULL, &matView);

        matView._41 = vPos.x;
        matView._42 = vPos.y;
        matView._43 = vPos.z;

        _matrix matScale;
        D3DXMatrixScaling(&matScale, 2.0f, 2.0f, 2.0f);
        matView = matScale * matView;

        m_pTransformCom->Set_WorldMatrix(&(matView));
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

        m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    }
    else
    {
        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

        _matrix   matWorld, matView, matBill;
        m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
        D3DXMatrixInverse(&matView, NULL, &matView);

        _matrix matScale;
        D3DXMatrixScaling(&matScale, 2.0f, 2.0f, 2.0f);

        for (int i = 0; i < 5; ++i)
        {
            _matrix matTmp;

            matView._41 = m_tHalluPoses[i].x;
            matView._42 = m_tHalluPoses[i].y;
            matView._43 = m_tHalluPoses[i].z;

            matTmp = matScale * matView;

            m_pTransformCom->Set_WorldMatrix(&(matTmp));
            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

            m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

            m_pBufferCom->Render_Buffer();
        }

        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    }

}
//void CBoss_Phase1::Render_GameObject()
//{
//    if (m_eBoss1State == BOSS1_DEAD && m_iMaxFrame <= (_uint)(m_fFrame))
//    {
//        return;
//    }
//
//    if (!m_bHallucinate)
//    {
//        _vec3 vPos = m_pTransformCom->Get_Pos();
//        _matrix   matWorld, matView, matBill;
//        m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
//        D3DXMatrixInverse(&matView, NULL, &matView);
//
//        matView._41 = vPos.x;
//        matView._42 = vPos.y;
//        matView._43 = vPos.z;
//
//        _matrix matScale;
//        D3DXMatrixScaling(&matScale, 2.0f, 2.0f, 2.0f);
//        matView = matScale * matView;
//
//        m_pTransformCom->Set_WorldMatrix(&(matView));
//        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
//
//        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//        m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//        m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
//
//        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
//
//        m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);
//
//        m_pBufferCom->Render_Buffer();
//
//        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
//        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//    }
//    else
//    {
//        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//        m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//        m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//        m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
//
//        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
//
//        _matrix   matWorld, matView, matBill;
//        m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
//        D3DXMatrixInverse(&matView, NULL, &matView);
//
//        _matrix matScale;
//        D3DXMatrixScaling(&matScale, 2.0f, 2.0f, 2.0f);
//
//        float angleStep = 2 * D3DX_PI / 5; // 360 degrees / 5
//        for (int i = 0; i < 5; ++i)
//        {
//            _matrix matTmp;
//            float angle = i * angleStep;
//            _vec3 vPos = m_pTransformCom->Get_Pos();
//            vPos.x += cos(angle) * 10.0f;
//            vPos.z += sin(angle) * 10.0f;
//
//            matView._41 = vPos.x;
//            matView._42 = vPos.y;
//            matView._43 = vPos.z;
//
//            matTmp = matScale * matView;
//
//            m_pTransformCom->Set_WorldMatrix(&(matTmp));
//            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
//
//            m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);
//
//            m_pBufferCom->Render_Buffer();
//        }
//
//        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
//        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//    }
//
//}

void CBoss_Phase1::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    OBJ_TYPE ot = pGameObject->Get_ObjType();

    if (OT_PLAYER_BULLET == ot)
    {
        if (m_bHallucinate)
        {
            m_bHalluDmged = true;
        }
        else if(!m_bSuperArmor)
        {
            Take_Dmg(dynamic_cast<CPlayerBullet*>(pGameObject)->Get_Dmg());

            //피격 이펙트 출력
            CGameObject* pEffect;
            _vec3 vPos = pGameObject->Get_TransformCom_Pointer()->Get_Pos();
            pEffect = CEffect::Create(m_pGraphicDev, FX_BLOOD_BLACK, vPos);

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
                pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.08f, 6, 7);
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Firework", pParticleObject);
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"ShamblerEffect", pEffect);
            }
        }
    }
    __super::Action_On_Collision(pGameObject, fTimeDelta);
}

void CBoss_Phase1::Action_On_Collision(vector<CLine*> pvecLines)
{
    CGameObject::Action_On_Collision(pvecLines);
}


HRESULT CBoss_Phase1::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    //Textures
    pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Phase1_IdleTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Phase1_IdleTexture", pComponent });

    pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Phase1_HitTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Phase1_HitTexture", pComponent });

    pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Phase1_AtkTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Phase1_AtkTexture", pComponent });

    pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_Phase1_DeadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Boss_Phase1_DeadTexture", pComponent });

    //Texture Ends

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

CBoss_Phase1* CBoss_Phase1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBoss_Phase1* pInstance = DBG_NEW CBoss_Phase1(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CBoss_Phase1::Free()
{
    CMonster::Free();
}

void CBoss_Phase1::Motion_Change()
{
    if (m_eBoss1State != m_ePreBoss1State)
    {
        switch (m_eBoss1State)
        {
        case BOSS1_IDLE:
        case BOSS1_RUN:
        {
            if (m_fFrame > 1.f)
                m_fFrame = 0.f;
            m_pTextureCom = m_pTexCom_Idle;
            m_iMaxFrame = 1;
            m_fFrameSpeed = 1.f;
        }
        break;
        case BOSS1_HIT:
        {
            if (m_fFrame > 1.f)
                m_fFrame = 0.f;
            m_pTextureCom = m_pTexCom_Hit;
            m_iMaxFrame = 1;
            m_fFrameSpeed = 1.f;
        }
        break;
        case BOSS1_ATK:
        case BOSS1_WARP:
        case BOSS1_MAGIC:
        case BOSS1_HALLUCINATE:
        {
            m_pTextureCom = m_pTexCom_Atk;
            m_iMaxFrame = 10;
            m_fFrameSpeed = 2.f;
            m_fAtkTimer = 0.f;
        }
        break;
        case BOSS1_DEAD:
        {
            CGameObject::Remove_Collider();
            m_bSuperArmor = true;
            m_pTextureCom = m_pTexCom_Dead;
            m_iMaxFrame = 20;
            m_fFrameSpeed = 7.f;
        }
        break;
        }

        m_ePreBoss1State = m_eBoss1State;
    }
}

void CBoss_Phase1::Boss_Phase1_Take_Dmg(const _float& fTimeDelta)
{
    if (m_bHallucinate) return;

    if (!m_bSuperArmor)
    {
        wstring szSoundFile = L"";

        if (0 < m_iCurHealth)
        {

            m_eBoss1State = BOSS1_HIT;
            Knock_Back(m_pPlayer, m_fSpeed * 0.8f, fTimeDelta);
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
            m_eBoss1State = BOSS1_DEAD;
            szSoundFile = L"Cthulhu_phase.wav";
            m_fFrame = 0.f;
        }

        Engine::Get_SoundMgr()->Play_Sound(szSoundFile, MONSTERSOUND, 0.56f);
        m_bHit = false;
    }
    else
    {
        m_iCurHealth = m_iPreHealth;
    }
}

void CBoss_Phase1::Boss_SlowBullet(_vec3 vSrc, _vec3 vDst, _float fAngle)
{
    CGameObject* pGameObject;

    pGameObject = CMonsterBullet::Create(m_pGraphicDev, BULLET_SLOW, vSrc, vDst);

    if (pGameObject != nullptr)
    {
        CTransform* transf = dynamic_cast<CTransform*>(pGameObject->Get_TransformCom_Pointer());
        //총알 크기 조정
        transf->Set_Scale({ 0.6f, 0.6f, 0.6f });

        Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BossBullet", pGameObject);
    }

    dynamic_cast<CMonsterBullet*>(pGameObject)->Set_Angle(fAngle);
}