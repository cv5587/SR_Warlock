#include "stdafx.h"
#include "..\Header\Exit.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Door.h"

CExit::CExit(LPDIRECT3DDEVICE9 pGraphicDev)
    : CInteractiveProp(pGraphicDev), m_bWarp(false), m_bTeleport(false)
{
}

CExit::CExit(const CExit& rhs)
    : CInteractiveProp(rhs), m_bWarp(rhs.m_bWarp), m_bTeleport(rhs.m_bTeleport)
{
    Ready_GameObject();
}

CExit::~CExit()
{
}

HRESULT CExit::Ready_GameObject()
{
    TexNamer();
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Pos(-35.f, 2.1f, 11.f);
    m_pTransformCom->Set_Angle({ 0.f, D3DXToRadian(90.f), 0.f });
    //m_ePortalID = m_ePortalID;
    m_vPrePos = m_pTransformCom->Get_Pos();

    return S_OK;
}

Engine::_int CExit::Update_GameObject(const _float& fTimeDelta)
{

    m_fFrame += 20.f * fTimeDelta;

    if (20.f < m_fFrame)
        m_fFrame = 0.f;

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    CGameObject::Update_GameObject(fTimeDelta);


    return 0;
}

void CExit::LateUpdate_GameObject(const float& fTimeDelta)
{

    Detect_Player();

    if (m_bWarp)
    {
        m_bTeleport = true;
    }
    if ((Engine::Get_DIKeyState(DIK_O) & 0x80))
    {
        m_bTeleport = true;
    }
    __super::LateUpdate_GameObject(fTimeDelta);


    _vec3   vPos = m_pTransformCom->Get_Pos();
    __super::Compute_ViewZ(vPos);

}

void CExit::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture((_uint)m_fFrame / 5);

    m_pBufferCom->Render_Buffer();

}

HRESULT CExit::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExitTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ExitTexture", pComponent });

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

void CExit::TexNamer()
{
}


void CExit::Detect_Player()
{
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

    _vec3   vPlayerPos = pPlayer->Get_TransformCom_Pointer()->Get_Pos();

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    if (0.5f > D3DXVec3Length(&(vPlayerPos - vPos)))
    {
        m_bWarp = true;
        if (m_bSound)
        {
            Engine::Get_SoundMgr()->Play_Sound(L"Teleporter_Staticloop.wav", EXITSOUND, 0.3f, true);
            m_bSound = false;
        }
    }
    else
    {
        Engine::Get_SoundMgr()->StopSound(EXITSOUND);
        m_bWarp = false;
    }
}

void CExit::Scene_Change()
{

}

void CExit::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
}

void CExit::Action_On_Collision(vector<CLine*> pvecLines)
{
}

CExit* CExit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CExit* pInstance = DBG_NEW CExit(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Exit Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CExit::Free()
{
    __super::Free();
}