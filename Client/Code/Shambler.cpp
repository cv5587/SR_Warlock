#include "stdafx.h"
#include "..\Header\Shambler.h"
#include "MonsterBullet.h"

CShambler::CShambler(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMonster(pGraphicDev), m_ePhase(SHAMBLER_NORMAL), m_iBulletCount(0)
{
}

CShambler::CShambler(const CShambler& rhs)
    : CMonster(rhs), m_ePhase(rhs.m_ePhase), m_iBulletCount(rhs.m_iBulletCount)
{
    Ready_GameObject();
}

CShambler::~CShambler()
{
}

HRESULT CShambler::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Scale({ 2.f, 2.f, 2.f });
    m_iMaxHealth = m_iCurHealth = 300;
    m_iDamage = 5;
    m_fSpeed = 3.f;
    m_fDtctRng = 20.f;
    m_fAtkRng = 3.f;
    m_bInit = false;
    m_bHit = false;
    m_bAtk = false;
    m_fAtkTimer = 0.f;
    m_fStiffTimer = 0.f;
    m_fStiffSecond = 3.f;
    m_fRadius = 0.7f;
    m_pTransformCom->Set_Pos(-1.f, 10.f, 37.f);
    m_vPrePos = m_pTransformCom->Get_Pos();
    m_szName = L"Shambler";

    m_eMiniBossState = MINIBOSS_IDLE;
    m_ePreMiniBossState = MINIBOSS_END;

    //충돌처리해줘야댐
    CGameObject::Push_CollisionMgr(OT_MONSTER, m_fRadius);

    return S_OK;
}
//Looker 보고 총알 업데이트
Engine::_int CShambler::Update_GameObject(const _float& fTimeDelta)
{

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    CGameObject::Update_GameObject(fTimeDelta);

    m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

    //버그 있으면 수정요청 바람..
    if (m_fFrame > _float(m_iMaxFrame) && (m_eMiniBossState == MINIBOSS_ATK || m_eMiniBossState == MINIBOSS_BASH || m_eMiniBossState == MINIBOSS_MAGIC))
    {
        m_bAtk = true;
        m_fFrame = 0.f;
    }

    else if (m_fFrame > _float(m_iMaxFrame) && m_eMiniBossState != MINIBOSS_DEAD)
        m_fFrame = 0.f;


    if (m_bHit)
    {
        Shambler_Take_Dmg(fTimeDelta);
    }

    switch (m_eMiniBossState)
    {
    case MINIBOSS_IDLE:
    {
        if (!m_bInit)
        {
            m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
            m_bInit = true;
        }

        if (m_fDtctRng > DistanceToPlayer(m_pPlayer))
        {
            wstring szSoundFile;
            int i = rand() % 15;
            if (5 > i)
            {
                szSoundFile = L"shambler_detect_1.wav";
            }
            else if (10 > i)
            {
                szSoundFile = L"shambler_detect_2.wav";
            }
            else
            {
                szSoundFile = L"shambler_detect_3.wav";
            }

            Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 1.f, m_pTransformCom->Get_Pos());

            m_eMiniBossState = MINIBOSS_RUN;
        }
    }
    break;
    case MINIBOSS_RUN:
    {
        if (m_fAtkRng > DistanceToPlayer(m_pPlayer))
        {
            m_fAtkTimer = 0.f;
            int i = rand() % 3;
            switch (i)
            {
            case 0:
                m_eMiniBossState = MINIBOSS_ATK;
                break;
            case 1:
                m_eMiniBossState = MINIBOSS_BASH;
                break;
            case 2:
                m_eMiniBossState = MINIBOSS_MAGIC;
                break;
            }

        }
        else
        {
            Chase_Player(m_pPlayer, fTimeDelta);
        }
    }
    break;
    case MINIBOSS_ATK:
    {
        if (m_fAtkRng < DistanceToPlayer(m_pPlayer))
        {
            m_eMiniBossState = MINIBOSS_RUN;
        }
        else
        {
            if (m_bAtk)
            {
                m_fAtkTimer = 0.f;
                m_pPlayer->Add_Health(-7);

                wstring szSoundFile;
                int i = rand() % 15;
                if (5 > i)
                {
                    szSoundFile = L"shambler_attack_1.wav";
                }
                else if (10 > i)
                {
                    szSoundFile = L"shambler_attack_2.wav";
                }
                else
                {
                    szSoundFile = L"shambler_attack_3.wav";
                }
                Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 1.f, m_pTransformCom->Get_Pos());

                m_bAtk = false;
            }
        }
    }
    break;
    case MINIBOSS_BASH:
    {
        if (m_fAtkRng < DistanceToPlayer(m_pPlayer))
        {
            m_eMiniBossState = MINIBOSS_RUN;
        }
        else
        {
            if (m_bAtk)
            {
                m_pPlayer->Add_Health(-15);
                wstring szSoundFile;
                int i = rand() % 15;
                if (5 > i)
                {
                    szSoundFile = L"shambler_attack_1.wav";
                }
                else if (10 > i)
                {
                    szSoundFile = L"shambler_attack_2.wav";
                }
                else
                {
                    szSoundFile = L"shambler_attack_3.wav";
                }
                Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 1.f, m_pTransformCom->Get_Pos());
                m_bAtk = false;
            }
        }
    }
    break;
    case MINIBOSS_MAGIC:
    {
        _float d = DistanceToPlayer(m_pPlayer);

        if (m_fAtkRng < d)
        {
            m_eMiniBossState = MINIBOSS_RUN;
        }
        else
        {
            if (m_bAtk)
            {
                m_fAtkTimer += 10 * fTimeDelta;
                _vec3 vFrom, vTo;
                m_pTransformCom->Get_Info(INFO_POS, &vFrom);
                D3DXVec3Normalize(&vTo, &(m_pPlayer->Get_Player_Eye() - vFrom));

                if (5.f < m_fAtkTimer && m_fAtkTimer <= 10.f && m_iBulletCount == 0)
                {
                    wstring szSoundFile;
                    int i = rand() % 15;
                    if (5 > i)
                    {
                        szSoundFile = L"shambler_attack_1.wav";
                    }
                    else if (10 > i)
                    {
                        szSoundFile = L"shambler_attack_2.wav";
                    }
                    else
                    {
                        szSoundFile = L"shambler_attack_3.wav";
                    }
                    Engine::Get_SoundMgr()->Play_Sound(szSoundFile, 1.f, m_pTransformCom->Get_Pos());

                    m_lBullets.push_back(CIceBullet::Create(m_pGraphicDev, rand() % 3, vFrom + vTo * d * 0.25f));
                    m_iBulletCount++;
                }
                else if (10.f < m_fAtkTimer && m_fAtkTimer <= 15.f && m_iBulletCount == 1)
                {
                    m_lBullets.push_back(CIceBullet::Create(m_pGraphicDev, rand() % 3, vFrom + vTo * d * 0.5f));
                    m_iBulletCount++;
                }
                else if (15.f < m_fAtkTimer && m_iBulletCount == 2)
                {
                    m_lBullets.push_back(CIceBullet::Create(m_pGraphicDev, rand() % 3, vFrom + vTo * d * 0.75f));
                    m_fAtkTimer = 0.f;
                    m_iBulletCount = 0;
                    m_bAtk = false;
                    m_eMiniBossState = MINIBOSS_RUN;
                }
            }

        }


    }
    break;
    case MINIBOSS_HIT:
    {
        m_fStiffTimer += 10 * fTimeDelta;
        if (m_fStiffSecond < m_fStiffTimer)
        {
            m_fStiffTimer = 0.f;
            m_eMiniBossState = MINIBOSS_RUN;
        }
    }
    break;
    case MINIBOSS_DEAD:
    {
        if (m_iMaxFrame <= (_uint)(m_fFrame))
        {
            switch (m_ePhase)
            {
            case SHAMBLER_NORMAL:
            {
                m_ePhase = SHAMBLER_HALFCUT;
                m_eMiniBossState = MINIBOSS_RUN;
            }
            break;
            case SHAMBLER_HALFCUT:
            {
                m_ePhase = SHAMBLER_FULLCUT;
                m_eMiniBossState = MINIBOSS_RUN;
            }
            break;
            case SHAMBLER_FULLCUT:
            {
                return OBJ_DEAD;
            }
            break;
            default:
                m_ePhase = SHAMBLER_END;
                break;
            }
        }
    }
    break;
    }

    //TODO ; 몬스터 불렛 리스트 업데이트

    if (!m_lBullets.empty())
    {
        for (auto iter : m_lBullets)
        {
            iter->Update_GameObject(fTimeDelta);
        }
    }
    m_pTransformCom->Set_Y(3.f);

    return OBJ_NOEVENT;
}

void CShambler::LateUpdate_GameObject(const float& fTimeDelta)
{
    Motion_Change();
    __super::LateUpdate_GameObject(fTimeDelta);

    if (!m_lBullets.empty())
    {
        for (auto iter = m_lBullets.begin(); iter != m_lBullets.end();)
        {
            if ((*iter)->Get_Dead())
            {
                Safe_Release(*iter);
                iter = m_lBullets.erase(iter);
            }
            else
            {
                (*iter)->LateUpdate_GameObject(fTimeDelta);
                iter++;
            }
        }
    }
}

void CShambler::Render_GameObject()
{
    //깔끔한 죽음

    if (m_eMiniBossState == MINIBOSS_DEAD)
    {
        if (m_iMaxFrame <= (_uint)(m_fFrame) && m_ePhase == SHAMBLER_FULLCUT)
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

void CShambler::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
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
    break;
    case OT_MONSTER:
    {
        //몬스터끼리 충돌
        _float d1, d2;
        _vec3 vSrc, vDst, vDir;

        vSrc = m_pTransformCom_Clone->Get_Pos();
        vDst = pGameObject->Get_TransformCom_Pointer()->Get_Pos();

        d1 = D3DXVec3Length(&(vSrc - vDst));
        d2 = m_fRadius + pGameObject->Get_Radius();

        //두 원의 반지름 더한 값이 위치값 뺀거보다 큼
        //== 충돌했다
        // 어떤 위치를 바꿀건가 : 침범한 쪽
        // 어떤 방향으로 미는가 : 당한쪽에서 침범한쪽 == pGO - this

        if (d1 < d2)
        {
            d2 = d2 - d1;
            /*D3DXVec3Normalize(&vDir, &(vDst - vSrc));
            vDir = vDir * d2;
            dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(vDir + vDst);
            */
            D3DXVec3Normalize(&vDir, &(vSrc - vDst));
            vDir = vDir * d2;
            m_pTransformCom->Set_Pos(vSrc + vDir);

            /*if (99 <= m_tFramePos.size())
               dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(*m_tFramePos[1]);*/

        }
    }
    break;
    case OT_FLOOR:
    case OT_CEILING:
    case OT_WALL:
    {
    }
    break;
    }
    __super::Action_On_Collision(pGameObject, fTimeDelta);
}

void CShambler::Action_On_Collision(vector<CLine*> pvecLines)
{
    CGameObject::Action_On_Collision(pvecLines);
}

HRESULT CShambler::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    //Textures
    //Normal
    pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerNormal_IdleTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerNormal_IdleTexture", pComponent });

    pComponent = m_pTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerNormal_RunTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerNormal_RunTexture", pComponent });

    pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerNormal_AtkTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerNormal_AtkTexture", pComponent });

    pComponent = m_pTexCom_Bash = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerNormal_BashTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerNormal_BashTexture", pComponent });

    pComponent = m_pTexCom_Magic = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerNormal_MagicTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerNormal_MagicTexture", pComponent });

    pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerNormal_HitTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerNormal_HitTexture", pComponent });

    pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerNormal_DeadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerNormal_DeadTexture", pComponent });


    //Halfcut
    pComponent = m_pHalfcutTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerHalfcut_RunTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerHalfcut_RunTexture", pComponent });

    pComponent = m_pHalfcutTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerHalfcut_AtkTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerHalfcut_AtkTexture", pComponent });

    pComponent = m_pHalfcutTexCom_Magic = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerHalfcut_MagicTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerHalfcut_MagicTexture", pComponent });

    pComponent = m_pHalfcutTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerHalfcut_HitTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerHalfcut_HitTexture", pComponent });

    pComponent = m_pHalfcutTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerHalfcut_DeadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerHalfcut_DeadTexture", pComponent });


    //Fullcut
    pComponent = m_pFullcutTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerFullcut_RunTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerFullcut_RunTexture", pComponent });

    pComponent = m_pFullcutTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerFullcut_AtkTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerFullcut_AtkTexture", pComponent });

    pComponent = m_pFullcutTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerFullcut_HitTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerFullcut_HitTexture", pComponent });

    pComponent = m_pFullcutTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShamblerFullcut_DeadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShamblerFullcut_DeadTexture", pComponent });

    //Texture Ends

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

CShambler* CShambler::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CShambler* pInstance = DBG_NEW CShambler(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CShambler::Free()
{
    CMonster::Free();
    for_each(m_lBullets.begin(), m_lBullets.end(), CDeleteObj());
    //Safe_Release(m_pTexCom_Bash);
    //Safe_Release(m_pTexCom_Magic);

    //Safe_Release(m_pHalfcutTexCom_Run);
    //Safe_Release(m_pHalfcutTexCom_Atk);
    //Safe_Release(m_pHalfcutTexCom_Magic);
    //Safe_Release(m_pHalfcutTexCom_Hit);
    //Safe_Release(m_pHalfcutTexCom_Dead);

    //Safe_Release(m_pFullcutTexCom_Run);
    //Safe_Release(m_pFullcutTexCom_Atk);
    //Safe_Release(m_pFullcutTexCom_Hit);
    //Safe_Release(m_pFullcutTexCom_Dead);
}

void CShambler::Motion_Change()
{
    if (m_eMiniBossState != m_ePreMiniBossState)
    {
        switch (m_ePhase)
        {
        case SHAMBLER_NORMAL:
        {
            switch (m_eMiniBossState)
            {
            case MINIBOSS_IDLE:
            {
                m_pTextureCom = m_pTexCom_Idle;
                m_iMaxFrame = 1;
                m_fFrameSpeed = 1.f;
            }
            break;
            case MINIBOSS_RUN:
            {
                m_pTextureCom = m_pTexCom_Run;
                m_iMaxFrame = 8;
                m_fFrameSpeed = 10.f;
            }
            break;
            case MINIBOSS_ATK:
            {
                m_fAtkRng = 5.f;
                m_pTextureCom = m_pTexCom_Atk;
                m_iMaxFrame = 14;
                m_fFrameSpeed = 10.f;
            }
            case MINIBOSS_BASH:
            {
                m_fAtkRng = 5.f;
                m_pTextureCom = m_pTexCom_Bash;
                m_iMaxFrame = 11;
                m_fFrameSpeed = 10.f;
            }
            break;
            case MINIBOSS_MAGIC:
            {
                m_fAtkRng = 15.f;
                m_pTextureCom = m_pTexCom_Magic;
                m_iMaxFrame = 12;
                m_fFrameSpeed = 10.f;
            }
            break;
            case MINIBOSS_HIT:
            {
                m_pTextureCom = m_pTexCom_Hit;
                m_iMaxFrame = 1;
                m_fFrameSpeed = 1.f;
            }
            break;
            case MINIBOSS_DEAD:
            {
                m_pTextureCom = m_pTexCom_Dead;
                m_iMaxFrame = 6;
                m_fFrameSpeed = 10.f;
            }
            break;
            }
        }
        break;
        case SHAMBLER_HALFCUT:
            switch (m_eMiniBossState)
            {
            case MINIBOSS_IDLE:
            case MINIBOSS_RUN:
            {
                m_pTextureCom = m_pHalfcutTexCom_Run;
                m_iMaxFrame = 8;
                m_fFrameSpeed = 10.f;
            }
            break;
            case MINIBOSS_ATK:
            case MINIBOSS_BASH:

            {
                m_fAtkRng = 5.f;
                m_pTextureCom = m_pHalfcutTexCom_Atk;
                m_iMaxFrame = 11;
                m_fFrameSpeed = 10.f;
            }
            break;
            case MINIBOSS_MAGIC:
            {
                m_fAtkRng = 15.f;
                m_pTextureCom = m_pHalfcutTexCom_Magic;
                m_iMaxFrame = 11;
                m_fFrameSpeed = 10.f;
            }
            break;
            case MINIBOSS_HIT:
            {
                m_pTextureCom = m_pHalfcutTexCom_Hit;
                m_iMaxFrame = 1;
                m_fFrameSpeed = 1.f;
            }
            break;
            case MINIBOSS_DEAD:
            {
                m_pTextureCom = m_pHalfcutTexCom_Dead;
                m_iMaxFrame = 7;
                m_fFrameSpeed = 10.f;
            }
            break;
            }
            break;
        case SHAMBLER_FULLCUT:
            switch (m_eMiniBossState)
            {
            case MINIBOSS_IDLE:
            case MINIBOSS_RUN:
            {
                m_pTextureCom = m_pFullcutTexCom_Run;
                m_iMaxFrame = 8;
                m_fFrameSpeed = 10.f;
            }
            break;
            case MINIBOSS_ATK:
            case MINIBOSS_BASH:
            {
                m_fAtkRng = 5.f;
                m_pTextureCom = m_pFullcutTexCom_Atk;
                m_iMaxFrame = 10;
                m_fFrameSpeed = 10.f;
            }
            case MINIBOSS_MAGIC:
            {
                m_fAtkRng = 5.f;
                m_pTextureCom = m_pFullcutTexCom_Atk;
                m_iMaxFrame = 10;
                m_fFrameSpeed = 10.f;
            }
            break;
            case MINIBOSS_HIT:
            {
                m_pTextureCom = m_pFullcutTexCom_Hit;
                m_iMaxFrame = 1;
                m_fFrameSpeed = 1.f;
            }
            break;
            case MINIBOSS_DEAD:
            {
                CGameObject::Remove_Collider();
                m_pTextureCom = m_pFullcutTexCom_Dead;
                m_iMaxFrame = 13;
                m_fFrameSpeed = 10.f;
            }
            break;
            }

            break;
        }


        m_ePreMiniBossState = m_eMiniBossState;

    }

}

void CShambler::Shambler_Take_Dmg(const _float& fTimeDelta)
{
    wstring szSoundFile = L"";

    if (m_eMiniBossState != MINIBOSS_DEAD)
    {

        switch (m_ePhase)
        {
        case SHAMBLER_NORMAL:
        {
            if (200 < m_iCurHealth)
            {
                m_eMiniBossState = MINIBOSS_HIT;
                Knock_Back(m_pPlayer, m_fSpeed * 0.8f, fTimeDelta);
                int i = rand() % 15;
                if (5 > i)
                {
                    szSoundFile = L"shambler_pain_1.wav";
                }
                else if (10 > i)
                {
                    szSoundFile = L"shambler_pain_2.wav";
                }
                else
                {
                    szSoundFile = L"shambler_pain_3.wav";
                }
            }
            else if (200 >= m_iCurHealth)
            {
                m_eMiniBossState = MINIBOSS_DEAD;
                m_fFrame = 0.f;

                int i = rand() % 10;
                if (5 > i)
                {
                    szSoundFile = L"shambler_death_1.wav";
                }
                else
                {
                    szSoundFile = L"shambler_death_2.wav";
                }
            }


        }
        break;
        case SHAMBLER_HALFCUT:
        {
            if (100 < m_iCurHealth)
            {
                m_eMiniBossState = MINIBOSS_HIT;
                Knock_Back(m_pPlayer, m_fSpeed * 0.8f, fTimeDelta);

                int i = rand() % 15;
                if (5 > i)
                {
                    szSoundFile = L"shambler_pain_1.wav";
                }
                else if (10 > i)
                {
                    szSoundFile = L"shambler_pain_2.wav";
                }
                else
                {
                    szSoundFile = L"shambler_pain_3.wav";
                }
            }
            else if (100 >= m_iCurHealth)
            {
                m_eMiniBossState = MINIBOSS_DEAD;
                m_fFrame = 0.f;

                int i = rand() % 10;
                if (5 > i)
                {
                    szSoundFile = L"shambler_death_1.wav";
                }
                else
                {
                    szSoundFile = L"shambler_death_2.wav";
                }
            }
        }
        break;
        case SHAMBLER_FULLCUT:
        {
            if (0 < m_iCurHealth)
            {
                m_eMiniBossState = MINIBOSS_HIT;
                Knock_Back(m_pPlayer, m_fSpeed * 0.8f, fTimeDelta);

                int i = rand() % 15;
                if (5 > i)
                {
                    szSoundFile = L"shambler_pain_1.wav";
                }
                else if (10 > i)
                {
                    szSoundFile = L"shambler_pain_2.wav";
                }
                else
                {
                    szSoundFile = L"shambler_pain_3.wav";
                }
            }
            else if (0 >= m_iCurHealth)
            {
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

                    Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"ShamblerDeathEffect", pGameObject);
                    CGameObject* pParticleObject;
                    _vec3 vPos = m_pTransformCom->Get_Pos();
                    pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.1f, 6, 0);
                    Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Firework", pParticleObject);
                    pParticleObject = CFirework::Create(m_pGraphicDev, (vPos), 0.4f, 1, 4);
                    Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"Gib", pParticleObject);
                }

                m_eMiniBossState = MINIBOSS_DEAD;
                m_fFrame = 0.f;

                int i = rand() % 10;
                if (5 > i)
                {
                    szSoundFile = L"shambler_death_1.wav";
                }
                else
                {
                    szSoundFile = L"shambler_death_2.wav";
                }
            }
        }
        break;
        }

        Engine::Get_SoundMgr()->Play_Sound(szSoundFile, MONSTERSOUND, 0.56f);
        m_bHit = false;
    }
}