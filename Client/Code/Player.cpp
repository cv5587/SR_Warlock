#include "stdafx.h"
#include "..\Header\Player.h"
#include "Export_System.h"

#include "Claymore.h"
#include "Minigun.h"
#include "Revolver.h"
#include "Rocket_Launcher.h"
#include "Shotgun.h"
#include "Staff.h"
#include "Reactor.h"

//for monster test
#include "Yeti.h"
#include "Looker.h"
#include "Flamer.h"
#include "BulletCase.h"
#include "Boss_Phase1.h"
#include "Item.h"
#include "ScreenEffect.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    : CCamera(pGraphicDev), m_iCurHealth(100), m_iMaxHealth(100), m_iCurMana(50), m_iMaxMana(50), m_iPreHealth(100),
    m_fSpeed(10.f), m_iExp(0), m_fCurSpeed(10.f), m_fSpeedTimer(0.f),
    m_bReverse(false), m_fReverseTimer(0.f), m_shakeAmount(2.f), m_fElapsedShakingTime(0.f), m_fHitDelay(0.f), m_fTilt(0.f)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
    : CCamera(rhs), m_iCurHealth(rhs.m_iCurHealth), m_iMaxHealth(rhs.m_iMaxHealth),
    m_iCurMana(rhs.m_iCurMana), m_iMaxMana(rhs.m_iMaxMana), m_fSpeed(rhs.m_fSpeed),
    m_iExp(rhs.m_iExp), m_fCurSpeed(rhs.m_fCurSpeed), m_fSpeedTimer(rhs.m_fSpeedTimer),
    m_bIsDamaged(rhs.m_bIsDamaged), m_iPreHealth(rhs.m_iPreHealth),
    m_fElapsedShakingTime(rhs.m_fElapsedShakingTime), m_fShakeYAngle(rhs.m_fShakeYAngle), m_shakeAmount(rhs.m_shakeAmount),
    m_bReverse(rhs.m_bReverse), m_fReverseTimer(0.f), m_eCurWeapon(rhs.m_eCurWeapon), m_bKey{ false,false,false,false }, m_fHitDelay(0.f)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject(const _vec3* pEye,
    const _vec3* pAngle,
    const _float& fFov, const _float& fAspect,
    const _float& fNear, const _float& fFar)
{

    FAILED_CHECK_RETURN(Add_Component(pEye, pAngle), E_FAIL);

    m_vEye = *pEye;
    m_vAngle = *pAngle;
    m_fFov = fFov;
    m_fAspect = fAspect;
    m_fNear = fNear;
    m_fFar = fFar;

    UpdateCamera(0.f);

    m_pTransformCom->Set_Pos(m_vEye);
    m_pTransformCom->Set_Angle(m_vAngle);

    m_pTransformCom->Update_Component(0.f);

    m_eCurWeapon = CLAYMORE;
    m_mWeapon[CLAYMORE] = CClaymore::Create(m_pGraphicDev);
    m_mWeapon[MINIGUN] = CMinigun::Create(m_pGraphicDev);
    m_mWeapon[REVOLVER] = CRevolver::Create(m_pGraphicDev);
    m_mWeapon[ROCKET_LAUNCHER] = CRocket_Launcher::Create(m_pGraphicDev);
    m_mWeapon[SHOTGUN] = CShotgun::Create(m_pGraphicDev);
    m_mWeapon[STAFF] = CStaff::Create(m_pGraphicDev);
    m_mWeapon[REACTOR] = CReactor::Create(m_pGraphicDev);

    m_bKey[DOOR_RED] = false;
    m_bKey[DOOR_YELLOW] = false;
    m_bKey[DOOR_BLUE] = false;
    m_bKey[DOOR_GREY] = true;

    Push_CollisionMgr(OT_PLAYER, 0.2f);
    Set_FireLight();
    FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

    return S_OK;
}

HRESULT CPlayer::Ready_Clone_GameObject(CPlayer& rhs, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vAngle)
{
    FAILED_CHECK_RETURN(Add_Component(&vPos, &vAngle), E_FAIL);

    m_vEye = vPos;
    m_vAngle = vAngle;
    m_fFov = rhs.Get_Player_Fov();
    m_fAspect = rhs.Get_Player_Aspect();
    m_fNear = rhs.Get_Player_Near();
    m_fFar = rhs.Get_Player_Far();

    UpdateCamera(0.f);

    m_pTransformCom->Set_Pos(m_vEye);
    m_pTransformCom->Set_Angle(m_vAngle);

    m_pTransformCom->Update_Component(0.f);

    m_mWeapon[CLAYMORE] = CClaymore::Clone_Create((*static_cast<CClaymore*>((rhs.Get_Weapon(CLAYMORE)))));
    m_mWeapon[MINIGUN] = CMinigun::Clone_Create((*static_cast<CMinigun*>((rhs.Get_Weapon(MINIGUN)))));
    m_mWeapon[REVOLVER] = CRevolver::Clone_Create((*static_cast<CRevolver*>((rhs.Get_Weapon(REVOLVER)))));
    m_mWeapon[ROCKET_LAUNCHER] = CRocket_Launcher::Clone_Create((*static_cast<CRocket_Launcher*>((rhs.Get_Weapon(ROCKET_LAUNCHER)))));
    m_mWeapon[SHOTGUN] = CShotgun::Clone_Create((*static_cast<CShotgun*>((rhs.Get_Weapon(SHOTGUN)))));
    m_mWeapon[STAFF] = CStaff::Clone_Create((*static_cast<CStaff*>((rhs.Get_Weapon(STAFF)))));
    m_mWeapon[REACTOR] = CReactor::Clone_Create((*static_cast<CReactor*>((rhs.Get_Weapon(REACTOR)))));

    m_bKey[DOOR_RED] = rhs.Get_Key(DOOR_RED);
    m_bKey[DOOR_YELLOW] = rhs.Get_Key(DOOR_YELLOW);
    m_bKey[DOOR_BLUE] = rhs.Get_Key(DOOR_BLUE);
    m_bKey[DOOR_GREY] = rhs.Get_Key(DOOR_GREY);


    Push_CollisionMgr(OT_PLAYER, 0.2);
    Set_FireLight();
    FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

    return S_OK;
}

Engine::_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
    m_fHitDelay += 90.f * fTimeDelta;

    if (m_iPreHealth > m_iCurHealth && m_fHitDelay > 90.f)
    {
       m_fHitDelay = 0.f;
       Engine::Get_Layer(L"UI")->Add_GameObject(L"HitEffect", CScreenEffect::Create(m_pGraphicDev, 14));
       m_iPreHealth = m_iCurHealth;
    }
    m_iCurMana = m_mWeapon[STAFF]->Get_CurBullet();

    if (m_fElapsedShakingTime >= 18.f)
    {
        m_fDieAwayShakingTime += fTimeDelta;
    }
    else if (m_fElapsedShakingTime >= 20.f)
    {
        m_bShaking = false;
        m_fDieAwayShakingTime = 0.f;
        m_fElapsedShakingTime = 0.f;
        m_fShakeYAngle = 0.f;
        m_pTransformCom->Set_YAngle(m_vAngle.y);
    }

    if (m_bShaking)
    {
        Shake_Camera(fTimeDelta);
    }

    m_pTransformCom->Update_Component(fTimeDelta);
    m_pTransformCom->Get_Info(INFO_UP, &m_vUp);
    m_vEye = m_pTransformCom->Get_Pos();
    _vec3 vLook;
    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

    m_vAt = m_vEye + vLook;

    _int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

    if (iExit != 0)
        return iExit;

    Set_View_Space();

    Mouse_Move();
    Key_Input(fTimeDelta);
    Change_Weapon();
    m_mWeapon[m_eCurWeapon]->Update_GameObject(fTimeDelta);

    UpdateCamera(fTimeDelta);

    return iExit;
}

void CPlayer::LateUpdate_GameObject(const float& fTimeDelta)
{
    m_mWeapon[m_eCurWeapon]->LateUpdate_GameObject(fTimeDelta);

    Engine::CCamera::LateUpdate_GameObject(fTimeDelta);
    Set_View_Space();

    //슬로우 / 가속 버프시 일정 시간 지나면 원래 속도로 되돌아옴 (마지막 디버프 기준)
    if (fabs(m_fCurSpeed - m_fSpeed) > std::numeric_limits<float>::epsilon())
    {
        m_fSpeedTimer += fTimeDelta * 5.f;
        if (10.f < m_fSpeedTimer)
        {
            m_fCurSpeed = m_fSpeed;
        }
    }

    if (m_bReverse)
    {
        Engine::Get_SoundMgr()->SetFrequency(5.f);
        _matrix matFlip;
        m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matView);
        D3DXMatrixRotationZ(&matFlip, D3DXToRadian(m_fTilt));


        if (bIncrease)
        {
            m_fTilt += fTimeDelta * 25.f;
            if (30.f < m_fTilt)
            {
                bIncrease = false;
            }
        }
        else
        {
            m_fTilt -= fTimeDelta * 25.f;
            if (-30.f > m_fTilt)
            {
                bIncrease = true;
            }
        }

        m_matView *= matFlip;
        m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
    }




    //if (!tmp)
    //{
    //   if (m_bReverse)      //카메라 좌우반전
    //   {

    //      D3DVIEWPORT9 vp1 = originalVP;
    //      vp1.Width /= 2;
    //      DWORD dwOldState;


    //      m_pGraphicDev->SetViewport(&vp1);
    //      //m_pGraphicDev->CreateTexture()
    //      // 오른쪽 절반 화면

    //      D3DVIEWPORT9 vp2 = originalVP;
    //      vp2.Width /= 2;
    //      vp2.X = vp2.Width;

    //      m_pGraphicDev->SetViewport(&vp2);

    //      tmp = true;

    //   }
    //}

    D3DXVECTOR3 vLook = m_vAt - m_pTransformCom->Get_Pos();

    Engine::Get_SoundMgr()->Update_Listener(m_pTransformCom->Get_Pos(), vLook, m_vUp); //리스너 위치 업데이트

}

void CPlayer::Set_FireLight()
{
}

void CPlayer::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    //플레이어가 밀려나야 하는 경우  CGameObject::Action_On_Collision(pGameObject, fTimeDelta); 사용하기
    if (pGameObject->Get_ObjType() == OT_ITEM)
    {
        CItem* pItem = dynamic_cast<CItem*>(pGameObject);
        ITEMTYPE eItemType = pItem->Get_ItemType();
        _int iItemNo = pItem->Get_ItemNo();
        Engine::Get_Layer(L"UI")->Add_GameObject(L"HitEffect", CScreenEffect::Create(m_pGraphicDev, 15));
        switch (eItemType)
        {
        case BULLET:
            Engine::Get_SoundMgr()->Play_Sound(L"GetItem_Bullet.wav", PROPSOUND, 0.5f);
            switch (iItemNo)
            {
            case 0:
                m_mWeapon[ROCKET_LAUNCHER]->Add_Bullet(pItem->Get_ItemValue());
                break;
            case 1:
                m_mWeapon[SHOTGUN]->Add_Bullet(pItem->Get_ItemValue());
                break;
            case 2:
                m_mWeapon[REVOLVER]->Add_Bullet(pItem->Get_ItemValue());
                break;
            case 3:
                m_mWeapon[MINIGUN]->Add_Bullet(pItem->Get_ItemValue());
                break;
            }
            break;
        case EXP:
            Engine::Get_SoundMgr()->Play_Sound(L"GetItem_Exp.wav", PROPSOUND, 0.5f);
            Add_Exp(pItem->Get_ItemValue());
            break;
        case HEALTH:
            Engine::Get_SoundMgr()->Play_Sound(L"GetItem_Potion.wav", PROPSOUND, 0.5f);
            Add_Health(pItem->Get_ItemValue());
            break;
        case KEY:
            Engine::Get_SoundMgr()->Play_Sound(L"GetItem_Key.wav", PROPSOUND, 0.5f);
            Add_Key(iItemNo);
            break;
        case MANA:
            Engine::Get_SoundMgr()->Play_Sound(L"GetItem_Potion.wav", PROPSOUND, 0.5f);
            m_mWeapon[STAFF]->Add_Bullet(pItem->Get_ItemValue());
            break;
        }
    }
    else if (pGameObject->Get_ObjType() == OT_NONPASSPROP)
    {
        //door, toggle에서는 벽충돌하듯이 따로 처리해줌
        if (pGameObject->Is_Plane() == false)
            CGameObject::Action_On_Collision(pGameObject, fTimeDelta);

    }
    // 피격 딜레이 설정(90.f : 1초)
    else if (m_fHitDelay > 60.f)
    {
        // 몬스터, 몬스터 총알 등 데미지 입는 오브젝트 처리
        if (pGameObject->Get_ObjType() == OT_MONSTER_BULLET)
        {
            Engine::Get_Layer(L"UI")->Add_GameObject(L"HitEffect", CScreenEffect::Create(m_pGraphicDev, 14));
            Add_Health(-dynamic_cast<CMonsterBullet*>(pGameObject)->Get_Damage());
            m_fHitDelay = 0.f;
        }
        else if (pGameObject->Get_ObjType() == OT_MONSTER)
        {
            CGameObject::Action_On_Collision(pGameObject, fTimeDelta);
        }
    }
    m_pTransformCom->Set_Y(2.1f);
}

void CPlayer::Calculate_Degree(_vec3 vPos)
{
    _vec3 vDir1, vDir2;
    m_pTransformCom->Get_Info(INFO_LOOK, &vDir1);
    vDir2 = vPos - m_pTransformCom->Get_Pos();

    D3DXVec3Normalize(&vDir1, &vDir1);
    D3DXVec3Normalize(&vDir2, &vDir2);

    float fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vDir1, &vDir2)));
    if (fAngle < 60.f) m_bNearby = true;
}

void CPlayer::Action_On_Collision(vector<CLine*> pvecLines)
{
    CGameObject::Action_On_Collision(pvecLines);
}

void CPlayer::UpdateCamera(const _float& fTimeDelta)
{
    _matrix identity;
    D3DXMatrixIdentity(&identity);


    for (_int i = 0; i < INFO_POS; ++i)
        memcpy(&m_vInfo[i], &identity.m[i][0], sizeof(_vec3));

    _matrix         matRot[ROT_END];

    D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
    D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y + m_fShakeYAngle);
    D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

    for (_int i = 0; i < INFO_POS; ++i)
    {
        for (_int j = 0; j < ROT_END; ++j)
        {
            D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
        }
    }

    m_vInfo[INFO_POS] = m_pTransformCom_Clone->Get_Pos();


    m_vUp = m_vInfo[INFO_UP];
    m_vEye = m_pTransformCom->Get_Pos();
    m_vAt = m_vEye + m_vInfo[INFO_LOOK];

    return;
}

void CPlayer::Shake_Camera(const _float& fTImeDelta)
{
    m_fShakeYAngle = sin(D3DXToRadian(float(rand() % 360))) * D3DX_PI * 0.01f * expf(-m_fDieAwayShakingTime * 2);
    m_pTransformCom->Set_YAngle(m_vAngle.y + m_fShakeYAngle);

    m_fElapsedShakingTime += fTImeDelta;
}

HRESULT CPlayer::Add_Component(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAngle)
{
    CComponent* pComponent = nullptr;
    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    m_pTransformCom->Set_Pos(*pEye);
    m_pTransformCom->Set_Angle(*pAngle);


    return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
    _vec3 vDir_Look;
    m_pTransformCom->Get_Info(INFO_LOOK, &vDir_Look);
    D3DXVec3Normalize(&vDir_Look, &vDir_Look);
    if (!m_bShopOn)
    {
        if (m_eCurWeapon == REACTOR && dynamic_cast<CReactor*>(m_mWeapon[m_eCurWeapon])->Get_Charge())
        {
            m_mWeapon[m_eCurWeapon]->Attack(m_vEye + vDir_Look, vDir_Look);
        }
        else if (Engine::Get_DIMouseState(DIM_LB))
        {
            m_mWeapon[m_eCurWeapon]->Attack(m_vEye + vDir_Look, vDir_Look);
        }
    }
    _matrix         matCamWorld;
    D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);
    if (Engine::Get_DIKeyState(DIK_R) & 0x80)
        m_bKey[0] = true;
    if (Engine::Get_DIKeyState(DIK_T) & 0x80)
        m_bKey[0] = false;
    if (Engine::Get_DIKeyState(DIK_F) & 0x80)
        m_bKey[1] = true;
    if (Engine::Get_DIKeyState(DIK_G) & 0x80)
        m_bKey[1] = false;
    if (Engine::Get_DIKeyState(DIK_V) & 0x80)
        m_bKey[2] = true;
    if (Engine::Get_DIKeyState(DIK_B) & 0x80)
        m_bKey[2] = false;

    if (m_iCurHealth < m_iMaxHealth && Engine::Get_DIKeyState(DIK_Q) & 0x80)
        m_iCurHealth++;
    else if (m_iCurHealth > 0 && Engine::Get_DIKeyState(DIK_E) & 0x80)
        m_iCurHealth--;

    if (m_iCurMana < m_iMaxMana && Engine::Get_DIKeyState(DIK_Z) & 0x80)
        m_iCurMana++;
    else if (m_iCurMana > 0 && Engine::Get_DIKeyState(DIK_C) & 0x80)
        m_iCurMana--;

    if (Engine::Get_DIKeyState(DIK_W) & 0x80)
    {
        _vec3 vLook;
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));
        vLook.y = 0.f;
        D3DXVec3Normalize(&vLook, &vLook);

        Move_Pos(vLook, m_fCurSpeed, fTimeDelta);
    }

    if (Engine::Get_DIKeyState(DIK_S) & 0x80)
    {
        _vec3 vLook;
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));
        vLook.y = 0.f;
        D3DXVec3Normalize(&vLook, &vLook);

        vLook = -vLook;
        Move_Pos(vLook, m_fCurSpeed, fTimeDelta);
    }

    if (Engine::Get_DIKeyState(DIK_D) & 0x80)
    {
        _vec3 vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
        vRight.y = 0.f;
        D3DXVec3Normalize(&vRight, &vRight);

        Move_Pos(vRight, m_fCurSpeed, fTimeDelta);
    }

    if (Engine::Get_DIKeyState(DIK_A) & 0x80)
    {
        _vec3 vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
        vRight.y = 0.f;
        D3DXVec3Normalize(&vRight, &vRight);

        vRight = -vRight;

        Move_Pos(vRight, m_fCurSpeed, fTimeDelta);
    }

    if (Engine::Get_DIKeyState(DIK_L) & 0x80)
    {
        if (!Key_L_Pressed)
        {
            Key_L_Pressed = true;
            m_ColliderToggle = !m_ColliderToggle;
        }
    }
    else
    {
        Key_L_Pressed = false;
    }

    if (m_pTransformCom->Get_Pos() != m_vPrePos)
    {
        m_bMoving = true;
    }
    else
    {
        m_bMoving = false;
    }

    //Monster Dmg Test
    if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
    {
        CYeti* yet = dynamic_cast<CYeti*>(Engine::Get_GameObject(L"GameLogic", L"Yeti"));
        yet->Take_Dmg(5);

        CLooker* look = dynamic_cast<CLooker*>(Engine::Get_GameObject(L"GameLogic", L"Looker"));
        look->Take_Dmg(5);


        CFlamer* flame = dynamic_cast<CFlamer*>(Engine::Get_GameObject(L"GameLogic", L"Flamer"));
        flame->Take_Dmg(5);

        CBoss_Phase1* boss = dynamic_cast<CBoss_Phase1*>(Engine::Get_GameObject(L"GameLogic", L"Boss1"));
        boss->Take_Dmg(5);
    }

    //test
    if (Engine::Get_DIKeyState(DIK_M) & 0x80)
    {
        Engine::Get_SoundMgr()->SetFrequency(2.f);
    }
}

void CPlayer::Change_Weapon()
{
    if (m_mWeapon[m_eCurWeapon]->Get_Attack()) return;
    if (Engine::Get_DIKeyState(DIKEYBOARD_1) && m_eCurWeapon != CLAYMORE) {
        Engine::Get_SoundMgr()->Play_Sound(L"Change_Weapon.wav", CHANGESOUND, 0.3f, false);
        m_eCurWeapon = CLAYMORE;
    }
    else if (Engine::Get_DIKeyState(DIKEYBOARD_2) && m_eCurWeapon != MINIGUN) {
        Engine::Get_SoundMgr()->Play_Sound(L"Change_Weapon.wav", CHANGESOUND, 0.3f, false);
        m_eCurWeapon = MINIGUN;
    }
    else if (Engine::Get_DIKeyState(DIKEYBOARD_3) && m_eCurWeapon != STAFF) {
        Engine::Get_SoundMgr()->Play_Sound(L"Change_Weapon.wav", CHANGESOUND, 0.3f, false);
        m_eCurWeapon = STAFF;
    }
    else if (Engine::Get_DIKeyState(DIKEYBOARD_4) && m_eCurWeapon != SHOTGUN) {
        Engine::Get_SoundMgr()->Play_Sound(L"Change_Weapon.wav", CHANGESOUND, 0.3f, false);
        m_eCurWeapon = SHOTGUN;
    }
    else if (Engine::Get_DIKeyState(DIKEYBOARD_5) && m_eCurWeapon != REVOLVER) {
        Engine::Get_SoundMgr()->Play_Sound(L"Change_Weapon.wav", CHANGESOUND, 0.3f, false);
        m_eCurWeapon = REVOLVER;
    }
    else if (Engine::Get_DIKeyState(DIKEYBOARD_6) && m_eCurWeapon != ROCKET_LAUNCHER) {
        Engine::Get_SoundMgr()->Play_Sound(L"Change_Weapon.wav", CHANGESOUND, 0.3f, false);
        m_eCurWeapon = ROCKET_LAUNCHER;
    }
    else if (Engine::Get_DIKeyState(DIKEYBOARD_7) && m_eCurWeapon != REACTOR) {
        Engine::Get_SoundMgr()->Play_Sound(L"Change_Weapon.wav", CHANGESOUND, 0.3f, false);
        m_eCurWeapon = REACTOR;
    }
}

void CPlayer::Mouse_Move()
{
    _matrix         matCamWorld;
    D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

    _long   dwMouseMove(0);

    if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
    {
        m_vAngle.x += D3DXToRadian(dwMouseMove / 10.f);
        if (m_vAngle.x >= D3DXToRadian(90.f))
            m_vAngle.x = D3DXToRadian(90.f);
        if (m_vAngle.x <= -D3DXToRadian(90.f))
            m_vAngle.x = -D3DXToRadian(90.f);

        m_pTransformCom->Set_Angle({ m_vAngle.x ,m_vAngle.y + m_fShakeYAngle, m_vAngle.z });
    }

    if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
    {
        m_vAngle.y += D3DXToRadian(dwMouseMove / 10.f);
        if (m_vAngle.y >= D3DXToRadian(360.f))
            m_vAngle.y -= D3DXToRadian(360.f);
        if (m_vAngle.y <= -D3DXToRadian(360.f))
            m_vAngle.y += D3DXToRadian(360.f);

        m_pTransformCom->Set_Angle({ m_vAngle.x ,m_vAngle.y + m_fShakeYAngle, m_vAngle.z });
    }


}


void CPlayer::Free()
{
    Engine::CCamera::Free();
    for_each(m_mWeapon.begin(), m_mWeapon.end(), CDeleteMap());
}

CPlayer* CPlayer::Clone_Create(CPlayer& rhs, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vAngle)
{
    CPlayer* pInstance = DBG_NEW CPlayer(rhs);

    if (FAILED(pInstance->Ready_Clone_GameObject(rhs, vPos, vAngle)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Player Clone Failed");
        return nullptr;
    }

    return pInstance;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAngle, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
    CPlayer* pInstance = DBG_NEW CPlayer(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject(pEye, pAngle, fFov, fAspect, fNear, fFar)))
    {
        Safe_Release(pInstance);
        MSG_BOX("dynamiccamera Create Failed");
        return nullptr;
    }

    return pInstance;
}