#include "stdafx.h"
#include "Reactor.h"

#include <PB_Reactor.h>

CReactor::CReactor(LPDIRECT3DDEVICE9 pGraphicDev)
    :CWeapon(pGraphicDev), m_bCharge(false), m_bShoot(false)
{
}

CReactor::CReactor(const CReactor& rhs)
    :CWeapon(rhs), m_bCharge(false), m_bShoot(false)
{
}

CReactor::~CReactor()
{
}

HRESULT CReactor::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_iCurBullet = 20;
    m_iMaxBullet = 20;
    m_iDamage = 50;
    return S_OK;
}

HRESULT CReactor::Ready_Clone_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    return S_OK;
}

_int CReactor::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bAttack)
    {
        if (144.f < m_fFrame)
        {
            m_fFrame = 0.f;
            m_bAttack = false;
            m_bShoot = false;
            m_bCharge = false;
        }
        else if (88.f < m_fFrame && !m_bShoot)
        {
            m_bCharge = true;
            m_bShoot = true;
        }
        else
        {
            m_fFrame += 90.f * fTimeDelta;
        }
    }
    CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CReactor::LateUpdate_GameObject(const float& fTimeDelta)
{
    __super::LateUpdate_GameObject(fTimeDelta);
}

void CReactor::Render_GameObject()
{
    m_pTextureCom->Set_Texture((_int)m_fFrame / 4);
}

void CReactor::Attack(_vec3 vPos, _vec3 vDir)
{
    if (!m_bAttack && m_iCurBullet > 0)
    {
        Engine::Get_SoundMgr()->Play_Sound(L"Shoot_Reactor2.wav", EFFECT, 0.27f);
        m_fLightFrame = m_fFrame;
        m_bAttack = true;
    }
    else if (m_bCharge)
    {
        m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_SHOOT, m_iLightIndex, true, vPos - vDir, 3.f);
        Engine::Get_SoundMgr()->Play_Sound(L"Shoot_Reactor.wav", EFFECT, 0.27f);
        --m_iCurBullet;
        m_bCharge = false;
        CPB_Reactor* pBullet = CPB_Reactor::Create(m_pGraphicDev, m_iDamage, 10.f);
        pBullet->Set_Dir(vDir);
        pBullet->Set_PrePos(vPos + vDir);
        pBullet->Set_Pos(vPos + vDir);
        Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"PB_None", pBullet);
    }
    else if (m_iCurBullet == 0)
    {
        Engine::Get_SoundMgr()->Play_Sound(L"Bullet_Empty.wav", PROPSOUND, 0.5f, false);
    }
}

HRESULT CReactor::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Reactor"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Reactor", pComponent });
    return S_OK;
}

CReactor* CReactor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CReactor* pInstance = DBG_NEW CReactor(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Reactor Create Failed");
        return nullptr;
    }

    return pInstance;
}

CReactor* CReactor::Clone_Create(const CReactor& rhs)
{
    CReactor* pInstance = DBG_NEW CReactor(rhs);

    if (FAILED(pInstance->Ready_Clone_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Reactor Clone Failed");
        return nullptr;
    }

    return pInstance;
}

void CReactor::Free()
{
    __super::Free();
}