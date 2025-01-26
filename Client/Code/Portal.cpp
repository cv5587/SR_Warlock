#include "stdafx.h"
#include "..\Header\Portal.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Door.h"
CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev, PORTALID ePortalID)
    : CInteractiveProp(pGraphicDev), m_bWarp(false), m_ePortalID(ePortalID), m_bSound(false)
{
}

CPortal::CPortal(const CPortal& rhs)
    : CInteractiveProp(rhs), m_bWarp(rhs.m_bWarp), m_ePortalID(rhs.m_ePortalID), m_bSound(rhs.m_bSound)
{
    Ready_GameObject();
}

CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Pos(7.f, 10.f, 7.f);
    m_bInit = false;
    m_bSound = false;
    m_vPrePos = m_pTransformCom->Get_Pos();

    return S_OK;
}

Engine::_int CPortal::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bInit)
    {
        m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
        m_bInit = true;
    }

    m_fFrame += 40.f * fTimeDelta;

    if (40.f < m_fFrame)
        m_fFrame = 0.f;

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    CGameObject::Update_GameObject(fTimeDelta);
    Detect_Player();

    return OBJ_NOEVENT;


}

void CPortal::LateUpdate_GameObject(const float& fTimeDelta)
{

    if (m_bWarp)
    {
        Warp_Player();
        m_bWarp = false;
    }

    __super::LateUpdate_GameObject(fTimeDelta);

    _vec3   vPos = m_pTransformCom->Get_Pos();
    __super::Compute_ViewZ(vPos);

}

void CPortal::Render_GameObject()
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

    m_pTextureCom->Set_Texture((_uint)m_fFrame / 5);

    m_pBufferCom->Render_Buffer();

}

HRESULT CPortal::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PortalTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_PortalTexture", pComponent });

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

void CPortal::TexNamer()
{
}


void CPortal::Detect_Player()
{
    _vec3 vPos = m_pTransformCom->Get_Pos();

    _float d = D3DXVec3Length(&(m_pPlayer->Get_TransformCom_Pointer()->Get_Pos() - vPos));

    if (4.f > d)
    {
        if (!m_bSound)
        {
            Engine::Get_SoundMgr()->Play_Sound(L"Teleporter_Staticloop.wav", WARPSOUND, 0.5f);
            m_bSound = true;
        }

        if (1.5f > d) //test
        {
            m_bWarp = true;
            Engine::Get_SoundMgr()->Play_Sound(L"Teleporter_Enter.wav", PROPSOUND, 0.5f);
        }
        else
        {
            m_bWarp = false;
        }
    }
    else
    {
        if (m_bSound)
        {
            Engine::Get_SoundMgr()->StopSound(WARPSOUND);
            m_bSound = false;
        }
    }

}

void CPortal::Warp_Player()
{
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

    _vec3    vDstPos;

    switch (m_ePortalID)
    {
    case PORTAL_A:
        vDstPos = { -4.f, m_pPlayer->Get_TransformCom_Pointer()->Get_Pos().y, 0.f };
        break;
    case PORTAL_B:
        vDstPos = { -27.5f, m_pPlayer->Get_TransformCom_Pointer()->Get_Pos().y, 10.83f };
        break;
    case PORTAL_C:
        break;
    default:
        break;
    }

    //eye, at ฟ๖วม
    pPlayer->Set_Player_Pos(vDstPos);


}

CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, PORTALID ePortalID)
{
    CPortal* pInstance = DBG_NEW CPortal(pGraphicDev, ePortalID);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Portal Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CPortal::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
}

void CPortal::Action_On_Collision(vector<CLine*> pvecLines)
{
}

void CPortal::Free()
{
    __super::Free();
}