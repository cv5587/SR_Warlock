#include "stdafx.h"
#include "ScreenEffect.h"

#include "Export_System.h"
#include "Player.h"

CScreenEffect::CScreenEffect(LPDIRECT3DDEVICE9 pGraphicDev, _int _iTexNum)
    :CUICamera(pGraphicDev), m_iTexNum(_iTexNum), m_iMaxFrame(0)
{
}

CScreenEffect::CScreenEffect(const CScreenEffect& rhs)
    : CUICamera(rhs), m_iTexNum(rhs.m_iTexNum), m_iMaxFrame(rhs.m_iMaxFrame)
{
}

CScreenEffect::~CScreenEffect()
{
}

HRESULT CScreenEffect::Ready_GameObject()
{
    Engine::CUICamera::Ready_UICamera();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    // ¸Ç À§¿¡ Ãâ·ÂÇÏ±â À§ÇØ °ª ¼³Á¤
    m_iSortOrder = 1555;
    m_pTransformCom->m_vScale.x = WINCX * 0.5f;
    m_pTransformCom->m_vScale.y = WINCY * 0.5f;
    m_pTransformCom->Set_Pos(_vec3(0.f, 0.f, 0.f));

    switch (m_iTexNum) //Ãß°¡ÇÔ
    {
    case 14:
        m_iMaxFrame = 90;
        break;
    case 15:
        m_iMaxFrame = 90;
        break;
    case 16:   //È¥¶õ
        m_iMaxFrame = 100;
        break;
    }
    return S_OK;
}

_int CScreenEffect::Update_GameObject(const _float& fTimeDelta)
{
    if (16 == m_iTexNum)
    {
        m_fFrame += 20 * fTimeDelta;
    }
    else
    {
        m_fFrame += 90 * fTimeDelta;
    }

    if (m_fFrame > _float(m_iMaxFrame)) //¹Ù²Þ
    {
        if (16 == m_iTexNum)
        {
            dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"))->Set_Player_ReverseOff();
        }
        return OBJ_DEAD;
    }
    Engine::Add_RenderGroup(RENDER_UI, this);
    Engine::CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CScreenEffect::LateUpdate_GameObject(const float& fTimeDelta)
{
    Engine::CUICamera::LateUpdate_GameObject(fTimeDelta);
}

void CScreenEffect::Render_GameObject()
{
    Engine::CUICamera::Render_GameObject();

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    switch (m_iTexNum)
    {
        // ÇÇ°Ý ÀÌÆåÆ®
    case 14:
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_int(180 - m_fFrame * 2), 255, 255, 255));
        break;
        // ¾ÆÀÌÅÛ È¹µæ ÀÌÆåÆ®
    case 15:
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_int(90 - m_fFrame), 255, 255, 255));
        break;

    case 16:
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_int(100 - m_fFrame), 255, 255, 255));
        break;
    default:
        break;
    }

    m_pTextureCom->Set_Texture(m_iTexNum);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CScreenEffect::Add_Component()
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

CScreenEffect* CScreenEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iTexNum)
{
    CScreenEffect* pInstance = new CScreenEffect(pGraphicDev, _iTexNum);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("ScreenEffect Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CScreenEffect::Free()
{
    Engine::CUICamera::Free();
}