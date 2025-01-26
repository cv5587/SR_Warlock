#include "stdafx.h"
#include "MonsterHpUI.h"

#include "Export_System.h"

CMonsterHpUI::CMonsterHpUI(LPDIRECT3DDEVICE9 pGraphicDev, wstring szName)
    :CUICamera(pGraphicDev), m_fRatio(0.f), m_szName(szName)
{
}

CMonsterHpUI::CMonsterHpUI(const CMonsterHpUI& rhs)
    : CUICamera(rhs), m_fRatio(0.f), m_szName(rhs.m_szName)
{
}

CMonsterHpUI::~CMonsterHpUI()
{
}

HRESULT CMonsterHpUI::Ready_GameObject()
{
    Engine::CUICamera::Ready_UICamera();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_iSortOrder = 987;

    m_pTransformCom[0]->m_vScale.x = 340.f * 0.5f;
    m_pTransformCom[0]->m_vScale.y = 18.f * 0.5f;
    m_pTransformCom[0]->Set_Pos(_vec3(480.f - WINCX * 0.5f, -30.f + WINCY * 0.5f, 0.0f));

    m_pTransformCom[1]->m_vScale.x = 320.f * 0.5f;
    m_pTransformCom[1]->m_vScale.y = 12.f * 0.5f;
    m_pTransformCom[1]->Set_Pos(_vec3(480.f - WINCX * 0.5f, -30.f + WINCY * 0.5f, 0.0f));

    return S_OK;
}

_int CMonsterHpUI::Update_GameObject(const _float& fTimeDelta)
{
    m_fFrame += 90.f * fTimeDelta;
    if (m_fFrame > 60.f) m_bActive = false;
    else m_bActive = true;
    Engine::Add_RenderGroup(RENDER_UI, this);
    Engine::CGameObject::Update_GameObject(fTimeDelta);
    for (int i = 1; i < sizeof(m_pTransformCom) / sizeof(m_pTransformCom[0]); i++)
    {
        if (m_pTransformCom[i])
        {
            m_pTransformCom[i]->Update_Component(fTimeDelta);
        }
    }
    return 0;
}

void CMonsterHpUI::LateUpdate_GameObject(const float& fTimeDelta)
{
    Engine::CUICamera::LateUpdate_GameObject(fTimeDelta);
    for (int i = 1; i < sizeof(m_pTransformCom) / sizeof(m_pTransformCom[0]); i++)
    {
        if (m_pTransformCom[i])
        {
            m_pTransformCom[i]->LateUpdate_Component(fTimeDelta);
        }
    }
}

void CMonsterHpUI::Render_GameObject()
{
    if (!m_bActive) return;
    Engine::CUICamera::Render_GameObject();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[0]->Get_WorldMatrix());
    m_pTextureCom->Set_Texture(11);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[1]->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    IDirect3DTexture9* pTexture = nullptr;
    IDirect3DTexture9* pTexture2 = nullptr;
    int iTexNum(12);
    pTexture = (IDirect3DTexture9*)m_pTextureCom->Get_Texture(iTexNum);
    pTexture2 = (IDirect3DTexture9*)m_pTextureCom->Get_Texture(iTexNum + 5);
    D3DLOCKED_RECT rect;
    D3DLOCKED_RECT rect2;
    D3DSURFACE_DESC desc;
    pTexture->GetLevelDesc(0, &desc);
    pTexture->LockRect(0, &rect, 0, 0);
    pTexture2->LockRect(0, &rect2, 0, 0);
    BYTE* pBuf;
    BYTE* pBuf2;
    pBuf = (BYTE*)rect.pBits;
    pBuf2 = (BYTE*)rect2.pBits;
    // 옮길 텍스처를 검정색으로 채워줌
    for (unsigned int y = 0; y < desc.Height; y++) {
        for (int x = 0; x < 2048; x++) {
            pBuf2[x + y * rect.Pitch] = 0;
        }
    }
    for (unsigned int y = 0; y < desc.Height; y++) {
        for (int x = 0; x < 1024 * m_fRatio; x++) {
            pBuf2[x + y * rect.Pitch] = pBuf[x + y * rect.Pitch];
        }
    }
    pTexture->UnlockRect(0);
    pTexture2->UnlockRect(0);

    m_pGraphicDev->SetTexture(0, pTexture2);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    Render_Text();
}

void CMonsterHpUI::Render_Text()
{
    Engine::Render_Font(L"Font_Name", m_szName, &_vec2(475 - m_szName.length() * 5, 23.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CMonsterHpUI::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTransformCom[0] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    pComponent = m_pTransformCom[1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HUD"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_HUD", pComponent });

    return S_OK;
}

CMonsterHpUI* CMonsterHpUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring szName)
{
    CMonsterHpUI* pInstance = new CMonsterHpUI(pGraphicDev, szName);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("MonsterHpUI Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CMonsterHpUI::Free()
{
    Engine::CUICamera::Free();
    for (int i = 0; i < sizeof(m_pTransformCom) / sizeof(Engine::CTransform*); ++i)
    {
        Safe_Release(m_pTransformCom[i]);
    }
}