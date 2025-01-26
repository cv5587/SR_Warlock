#include "stdafx.h"
#include "..\Header\ShopNPC.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"
#include "ShopUICamera.h"
CShopNPC::CShopNPC(LPDIRECT3DDEVICE9 pGraphicDev)
    : CInteractiveProp(pGraphicDev), m_bShopOn(false)
{

}

CShopNPC::CShopNPC(const CShopNPC& rhs)
    : CInteractiveProp(rhs), m_bShopOn(rhs.m_bShopOn)
{
}

CShopNPC::~CShopNPC()
{
}

void CShopNPC::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
}

void CShopNPC::Action_On_Collision(vector<CLine*> pvecLines)
{
}

HRESULT CShopNPC::Ready_GameObject()
{
    TexNamer();
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_bInit = false;

    m_pTransformCom->Set_Pos({ -23.f, 2.f, 13.5f });
    m_vPrePos = m_pTransformCom->Get_Pos();

    m_eObjType = OT_NONPASSPROP;

    m_lowerBound = m_vPrePos - D3DXVECTOR3{ 1.f, 1.f, 1.f };
    m_upperBound = m_vPrePos + D3DXVECTOR3{ 1.f, 1.f, 1.f };

    m_bColliderIsNone = true;

    return S_OK;
}


Engine::_int CShopNPC::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bInit)
    {
        m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
        m_bInit = true;
    }

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    CGameObject::Update_GameObject(fTimeDelta);


    if (m_bShopOn)
    {
        CShopUICamera* shopUI = dynamic_cast<CShopUICamera*>(Engine::Get_GameObject(L"UI", L"ShopUICamera"));
        //ShowShop
        if (!shopUI->Get_ShowShop())
        {
            shopUI->Set_ShowShop();
        }
    }

    Detect_Player();


    return OBJ_NOEVENT;
}

void CShopNPC::LateUpdate_GameObject(const float& fTimeDelta)
{

    __super::LateUpdate_GameObject(fTimeDelta);

    _vec3   vPos = m_pTransformCom->Get_Pos();
    __super::Compute_ViewZ(vPos);

}

void CShopNPC::Render_GameObject()
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

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

}

HRESULT CShopNPC::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShopNPCTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_ShopNPCTexture", pComponent });

    return S_OK;
}

void CShopNPC::TexNamer()
{
}

void CShopNPC::Detect_Player()
{

    _vec3 vPos, vPlayerPos;
    vPlayerPos = m_pPlayer->Get_TransformCom_Pointer()->Get_Pos();
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    if (3.f > D3DXVec3Length(&(vPlayerPos - vPos)))
    {
        m_pPlayer->Calculate_Degree(m_pTransformCom->Get_Pos());
        if ((Engine::Get_DIKeyState(DIK_SPACE) & 0x80))
        {
            m_pPlayer->Set_Nearby(false);
            m_bShopOn = true;
            m_pPlayer->Set_ShopOn(m_bShopOn);
        }
        else
        {
            m_bShopOn = false;
        }

    }
}

CShopNPC* CShopNPC::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CShopNPC* pInstance = DBG_NEW CShopNPC(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Door Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CShopNPC::Free()
{
    __super::Free();
}