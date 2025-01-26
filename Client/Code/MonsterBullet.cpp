#include "stdafx.h"
#include "..\Header\MonsterBullet.h"

#include "Export_Utility.h"
#include "Fire.h"
#include "ScreenEffect.h"
CMonsterBullet::CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev, BULLETID eBulletID, const _vec3& vFrom, const _vec3& vTo)
    : Engine::CGameObject(pGraphicDev), m_bDead(false), m_bInit(false), m_eBulletID(eBulletID), m_vFrom(vFrom), m_vTo(vTo),
    m_fSpeed(1.f), m_fAngle(0.f), m_iFireCount(0), m_bTracker(false), m_fKillTimer(0.f)
{
}

CMonsterBullet::CMonsterBullet(const CMonsterBullet& rhs)
    : Engine::CGameObject(rhs)
{

}

CMonsterBullet::~CMonsterBullet()
{
}

HRESULT CMonsterBullet::Ready_GameObject()
{
    switch (m_eBulletID)
    {
    case BULLET_LOOKER:
        m_szTexName = L"Proto_LookerBulletTexture";
        m_iMaxFrame = 5;
        m_fFrameSpeed = 3.f;
        m_fSpeed = 8.f;
        m_iDamage = 3;
        m_fRadius = 0.5f;
        break;
    case BULLET_FLAMER:
        m_szTexName = L"Proto_FlamerBulletTexture";
        m_iMaxFrame = 8;
        m_fFrameSpeed = 8.f;
        m_iDamage = 4;
        m_fSpeed = 6.f;
        m_fRadius = 0.5f;

        break;
    case BULLET_SLOW:
        m_szTexName = L"Proto_SlowBulletTexture";
        m_iMaxFrame = 9;
        m_fFrameSpeed = 10.f;
        m_iDamage = 10; //보스가 쓰면 전방ㅇ향빌보드 써야하는가 . . .
        m_fSpeed = 5.f;
        m_fRadius = 1.f;
        break;
    case BULLET_MAGIC:
        m_szTexName = L"Proto_MagicBulletTexture";
        m_iMaxFrame = 14;
        m_fFrameSpeed = 10.f;
        m_iDamage = 10;
        m_fSpeed = 5.f;
        m_fRadius = 0.5f;
        break;
    case BULLET_SPIRAL:
        m_szTexName = L"Proto_SpiralBulletTexture";
        m_iMaxFrame = 8;
        m_fFrameSpeed = 10.f;
        m_iDamage = 15;
        m_fSpeed = 5.f;
        m_fRadius = 0.5f;
        break;
    default:
        break;
    }

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(m_vFrom);
    if (m_eBulletID != BULLET_FLAMER)
        m_pTransformCom->Set_Scale({ 0.3f, 0.3f, 0.3f });
    else
        m_pTransformCom->Set_Scale({ 0.8f, 0.8f, 0.8f });

    Push_CollisionMgr(OT_MONSTER_BULLET, m_fRadius);

    D3DXVec3Normalize(&m_vGo, &(m_vTo - m_vFrom));

    return S_OK;
}
Engine::_int CMonsterBullet::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bInit)
    {
        _matrix matRotation;
        D3DXMatrixRotationY(&matRotation, D3DXToRadian(-m_fAngle));
        D3DXVec3TransformCoord(&m_vGo, &m_vGo, &matRotation);

        if (m_bTracker)
        {
            m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
        }
        m_bInit = true;
    }

    if (m_bDead) return OBJ_DEAD;
    m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

    //버그 있으면 수정요청 바람..
    if (m_fFrame > m_iMaxFrame)
    {
        m_fFrame = 0.f;
        if (m_eBulletID == BULLET_FLAMER)
        {
            m_bDead = true;
            return OBJ_DEAD;
        }
    }


    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    //_vec3 vAngle = { 0.f, 0.f, 0.f };

    //왜 빌보드 적용하면 회전이 맛가는가

    CGameObject::Update_GameObject(fTimeDelta);
    if (m_bTracker)
    {
        m_fKillTimer += fTimeDelta;
        if (5.f < m_fKillTimer)
        {
            return OBJ_DEAD;
        }
        else
        {
            Chase_Player(m_pPlayer, fTimeDelta);
        }
    }
    else
    {
        Move_Pos(m_vGo, m_fSpeed, fTimeDelta);
    }

    m_pTransformCom->Update_Component(fTimeDelta);

    return OBJ_NOEVENT;
}

void CMonsterBullet::LateUpdate_GameObject(const float& fTimeDelta)
{

    __super::LateUpdate_GameObject(fTimeDelta);
}

void CMonsterBullet::Render_GameObject()
{
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


void CMonsterBullet::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    OBJ_TYPE ot = pGameObject->Get_ObjType();
    switch (ot)
    {
    case OT_PLAYER:
    {
        switch (m_eBulletID)
        {
        case BULLET_SLOW:
            dynamic_cast<CPlayer*>(pGameObject)->Set_Player_Speed(0.5f);
            break;
        case BULLET_MAGIC:
            m_pPlayer = dynamic_cast<CPlayer*>(pGameObject);
            if (!m_pPlayer->Get_Player_Reverse())
            {
                Engine::Get_Layer(L"UI")->Add_GameObject(L"DazeEffect", CScreenEffect::Create(m_pGraphicDev, 16));
                m_pPlayer->Set_Player_Tilt();
            }
            m_pPlayer->Set_Player_Reverse(); //추가함
           
            break;
        }
        //dynamic_cast<CPlayer*>(pGameObject)->Add_Health(-m_iDamage);
        m_bDead = true;
    }
    break;
    default:
    {

    }
    break;
    }
}
void CMonsterBullet::Action_On_Collision(vector<CLine*> pvecLines)
{
    if (BULLET_FLAMER == m_eBulletID)
    {
        if (m_iFireCount < 1)
        {
            CGameObject* pFire;
            _vec3 vPos;
         //   vPos = ;
            pFire = CFire::Create(m_pGraphicDev);

            if (pFire != nullptr)
            {
               // dynamic_cast<CTransform*>(pFire->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(m_pTransformCom->Get_Pos() + _vec3{ _float(rand() % 5), 0.f, _float(rand() % 5) });
                pFire->Get_TransformCom_Pointer()->Set_Pos(m_pTransformCom->Get_Pos());
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"ExplosionFire", pFire);
                ++m_iFireCount;
            }
        }
    }
    m_bDead = true;
   
}


HRESULT CMonsterBullet::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ m_szTexName.c_str(), pComponent });

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    //왜 다이나믹으로 바꾸면 출력이 안되는가

    return S_OK;
}

_float CMonsterBullet::DistanceToPlayer(CPlayer* pPlayer)
{
    _vec3 vPos;
    _matrix* vWorld;

    vWorld = m_pTransformCom->Get_WorldMatrix();

    vPos.x = vWorld->m[3][0];
    vPos.y = vWorld->m[3][1];
    vPos.z = vWorld->m[3][2];

    return D3DXVec3Length(&(pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vPos));
}

void CMonsterBullet::Chase_Player(CPlayer* pPlayer, const _float& fTimeDelta)
{

    //플레이어 위치 받아옴
    _vec3 vPos = m_pTransformCom->Get_Pos();
    vPos = pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vPos;
    Move_Pos(vPos, m_fSpeed, fTimeDelta);
}

void CMonsterBullet::Knock_Back(CPlayer* pPlayer, const _float& fSpeed, const _float& fTimeDelta)
{
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    vPos = vPos - pPlayer->Get_Player_Eye();
    Move_Pos(vPos, fSpeed, fTimeDelta);
}


CMonsterBullet* CMonsterBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, BULLETID eBulletID, const _vec3& vFrom, const _vec3& vTo)
{
    CMonsterBullet* pInstance = DBG_NEW CMonsterBullet(pGraphicDev, eBulletID, vFrom, vTo);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("MonsterBullet Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CMonsterBullet::Free()
{
    __super::Free();
}