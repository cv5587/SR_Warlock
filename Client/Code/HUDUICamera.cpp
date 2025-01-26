#include "stdafx.h"
#include "HUDUICamera.h"

#include "Export_System.h"

CHUDUICamera::CHUDUICamera(LPDIRECT3DDEVICE9 pGraphicDev, _int iTexNum)
    :CUICamera(pGraphicDev), m_iTexNum(iTexNum)
{
}

CHUDUICamera::CHUDUICamera(const CHUDUICamera& rhs)
    :CUICamera(rhs), m_iTexNum(rhs.m_iTexNum)
{
}

CHUDUICamera::~CHUDUICamera()
{
}

HRESULT CHUDUICamera::Ready_GameObject(const _float& fWidth, const _float& fHeight,
    const _float& fPosX, const _float& fPosY)
{
    Engine::CUICamera::Ready_UICamera();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    // 맨 위에 출력하기 위해 값 설정
    m_iSortOrder = m_iTexNum;
    m_pTransformCom->m_vScale.x = fWidth * 0.5f;
    m_pTransformCom->m_vScale.y = fHeight * 0.5f;
    m_pTransformCom->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

    return S_OK;
}

_int CHUDUICamera::Update_GameObject(const _float& fTimeDelta)
{
    Engine::Add_RenderGroup(RENDER_UI, this);
    Engine::CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CHUDUICamera::LateUpdate_GameObject(const float& fTimeDelta)
{
    Engine::CUICamera::LateUpdate_GameObject(fTimeDelta);
}

void CHUDUICamera::Render_GameObject()
{
    Engine::CUICamera::Render_GameObject();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(m_iTexNum);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CHUDUICamera::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HUD"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_HUD", pComponent });

    return S_OK;
}

CHUDUICamera* CHUDUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fWidth,
    const _float& fHeight, const _float& fPosX, const _float& fPosY, const _uint& uTexNum)
{
    CHUDUICamera* pInstance = new CHUDUICamera(pGraphicDev, uTexNum);

    if (FAILED(pInstance->Ready_GameObject(fWidth, fHeight, fPosX, fPosY)))
    {
        Safe_Release(pInstance);
        MSG_BOX("GameUICamera Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CHUDUICamera::Free()
{
    Engine::CUICamera::Free();
}