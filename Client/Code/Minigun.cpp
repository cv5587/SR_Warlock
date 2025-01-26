#include "stdafx.h"
#include "Minigun.h"

#include <PB_None.h>

CMinigun::CMinigun(LPDIRECT3DDEVICE9 pGraphicDev)
    :CWeapon(pGraphicDev), m_iAttackDelay(80)
{
}

CMinigun::CMinigun(const CMinigun& rhs)
    :CWeapon(rhs), m_iAttackDelay(rhs.m_iAttackDelay)
{
}

CMinigun::~CMinigun()
{
}

HRESULT CMinigun::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iCurBullet = 200;
    m_iMaxBullet = 200;
    m_iDamage = 5;
    m_iUpgrade_cost = 5;
    return S_OK;
}

HRESULT CMinigun::Ready_Clone_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    if (m_bUpgraded) Upgrade_Weapon();
    return S_OK;
}

_int CMinigun::Update_GameObject(const _float& fTimeDelta)
{
    __super::Update_GameObject(fTimeDelta);
    if (m_bAttack)
    {
        m_fFrame += 90.f * fTimeDelta;
        if (!m_bUpgraded)
        {
            if (14.f < m_fFrame)
            {
                m_fFrame = 0.f;
                m_bAttack = false;
            }
        }
        else
        {
            if (28.f < m_fFrame)
            {
                m_fFrame = 0.f;
                m_bAttack = false;
            }
        }
    }
    CGameObject::Update_GameObject(fTimeDelta);

    return 0;
}

void CMinigun::LateUpdate_GameObject(const float& fTimeDelta)
{
    __super::LateUpdate_GameObject(fTimeDelta);
}

void CMinigun::Render_GameObject()
{
    m_pTextureCom->Set_Texture((_int)(m_fFrame / 3.5f));
}

void CMinigun::Attack(_vec3 vPos, _vec3 vDir)
{
    _vec3 vLook, vUp, vRight, vBulletCaseDir;
    _vec3 vWorld_Up = { 0.f, 1.f, 0.f };

    D3DXVec3Normalize(&vLook, &vDir);

    D3DXVec3Cross(&vRight, &vWorld_Up, &vLook);
    D3DXVec3Normalize(&vRight, &vRight);

    D3DXVec3Cross(&vUp, &vLook, &vRight);
    D3DXVec3Normalize(&vUp, &vRight);


    vBulletCaseDir = vLook + vUp + vRight;
    vBulletCaseDir.x *= (rand() % 5 + 6) / 10.f;
    vBulletCaseDir.y *= (rand() % 5 + 6) / 10.f;
    vBulletCaseDir.z *= (rand() % 5 + 6) / 10.f;
    D3DXVec3Normalize(&vBulletCaseDir, &vBulletCaseDir);

    m_vBulletCaseDir = vBulletCaseDir;

    if (!((_int)m_fFrame % m_iAttackDelay) && m_iCurBullet > 0)
    {
        Engine::Get_SoundMgr()->Play_Sound(L"Shoot_Minigun.wav", EFFECT, 0.27f);
        m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_SHOOT, m_iLightIndex, true, vPos - vDir, 3.f);

        m_fLightFrame = m_fFrame;
        --m_iCurBullet;
        CPB_None* pBullet = CPB_None::Create(m_pGraphicDev, m_iDamage);
        pBullet->Set_Dir(vDir);
        pBullet->Set_PrePos(vPos - 60.f * vDir * Get_TimeDelta(L"Timer_FPS60"));
        pBullet->Set_Pos(vPos - 60.f * vDir * Get_TimeDelta(L"Timer_FPS60"));
        Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_None", pBullet);

        CBulletCase* pBulletCase = CBulletCase::Create(m_pGraphicDev);
        pBulletCase->Set_Dir(m_vBulletCaseDir);
        pBulletCase->Set_PrePos(vPos);
        static_cast<CBulletCase*>(pBulletCase)->Set_Pos(vPos);
        Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BulletCase", pBulletCase);

        m_bAttack = true;
    }
    else if (m_iCurBullet == 0)
    {
        Engine::Get_SoundMgr()->Play_Sound(L"Bullet_Empty.wav", PROPSOUND, 0.5f, false);
    }
}

void CMinigun::Upgrade_Weapon()
{
    m_bUpgraded = true;
    m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_TurboMinigun"));
    m_mapComponent[ID_STATIC].insert({ L"Proto_Player_TurboMinigun", m_pTextureCom });
    m_iDamage = 8;
    m_iAttackDelay = 5;
    m_iCurBullet = 400;
    m_iMaxBullet = 400;
}

HRESULT CMinigun::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Minigun"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Minigun", pComponent });

    return S_OK;
}

CMinigun* CMinigun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMinigun* pInstance = new CMinigun(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Minigun Create Failed");
        return nullptr;
    }

    return pInstance;
}

CMinigun* CMinigun::Clone_Create(const CMinigun& rhs)
{
    CMinigun* pInstance = DBG_NEW CMinigun(rhs);

    if (FAILED(pInstance->Ready_Clone_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Minigun Clone Failed");
        return nullptr;
    }

    return pInstance;
}

void CMinigun::Free()
{
    __super::Free();
}