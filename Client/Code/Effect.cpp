#include "stdafx.h"
#include "..\Header\Effect.h"

#include "Export_System.h"
#include "Export_Utility.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev, FXID eID, _vec3 vPos)
    : Engine::CGameObject(pGraphicDev), m_eFXID(eID), m_vStart(vPos)
{
}

CEffect::CEffect(const CEffect& rhs)
    : Engine::CGameObject(rhs), m_eFXID(rhs.m_eFXID), m_vStart(rhs.m_vStart)
{
    Ready_GameObject();
}

CEffect::~CEffect()
{
}

HRESULT CEffect::Ready_GameObject()
{
    TexNamer();
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Pos(m_vStart);

    switch (m_eFXID)
    {
    case FX_MAGICBLAST:
        m_pTransformCom->Set_Scale({ 0.6f, 0.6f, 0.6f });
        break;
    case FX_BULLET:
        m_pTransformCom->Set_Scale({ 0.2f, 0.2f, 0.2f });
        break;
    case FX_ROCKET:
        m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_ROCKET, m_iLightIndex, true, m_pTransformCom->Get_Pos(), 3.f);
        break;
    }

    return S_OK;
}

Engine::_int CEffect::Update_GameObject(const _float& fTimeDelta)
{
    /*m_fFrame += 90.f * fTimeDelta;

    if (90.f < m_fFrame)
    {
       m_fFrame = 0.f;
       return OBJ_DEAD;
    }*/

    m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

    //버그 있으면 수정요청 바람..
    if (m_fFrame > _float(m_iMaxFrame))
    {
        if (m_eFXID == FX_ROCKET)
        {
            m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_ROCKET, m_iLightIndex, FALSE);
        }
        else if (m_eFXID == FX_MAGICBLAST)
        {
            //m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_REACTOR, m_iLightIndex, FALSE);
        }
        else if (m_eFXID == FX_STAFF) return S_OK;
        return OBJ_DEAD;
    }
        
    CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    return 0;
}

void CEffect::LateUpdate_GameObject(const float& fTimeDelta)
{
    __super::LateUpdate_GameObject(fTimeDelta);

    __super::Compute_ViewZ(m_pTransformCom->Get_Pos());

}

void CEffect::Render_GameObject()
{
    if (FX_BLOOD_BLACK == m_eFXID)
    {
        _vec3 vPos = m_pTransformCom->Get_Pos();
        _matrix   matWorld, matView, matBill;
        m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
        D3DXMatrixInverse(&matView, NULL, &matView);

        matView._41 = vPos.x;
        matView._42 = vPos.y;
        matView._43 = vPos.z;

        _matrix matScale;
        D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.8f);
        matView = matScale * matView;

        m_pTransformCom->Set_WorldMatrix(&(matView));
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    }
    else
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
    }
    
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    // m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    if (m_eFXID == FX_MAGICBLAST)
    {
       // m_iLightIndex = Engine::Enable_Light(m_pGraphicDev, LIGHT_REACTOR, m_iLightIndex, TRUE, m_pTransformCom->Get_Pos(), 3.f);
    }

    m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

    m_pBufferCom->Render_Buffer();

    // m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect::Add_Component()
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

    return S_OK;
}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, FXID eID, _vec3 vPos)
{
    CEffect* pInstance = new CEffect(pGraphicDev, eID, vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CEffect::Free()
{
    __super::Free();
    Safe_Release(m_pBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}

void CEffect::TexNamer()
{
    switch (m_eFXID)
    {
    case FX_OVERKILL:
        m_szTexName = L"Proto_FX_OverkillTexture";
        m_iMaxFrame = 18;
        m_fFrameSpeed = 10.f;
        break;
    case FX_FIREDEATH:
        m_szTexName = L"Proto_FX_FiredeathTexture";
        m_iMaxFrame = 22;
        m_fFrameSpeed = 10.f;
        break;
    case FX_MAGICDEATH:
        m_szTexName = L"Proto_FX_MagicdeathTexture";
        m_iMaxFrame = 9;
        m_fFrameSpeed = 10.f;
        break;
    case FX_MAGICBOOM:
        m_szTexName = L"Proto_FX_MagicboomTexture";
        m_iMaxFrame = 7;
        m_fFrameSpeed = 10.f;
        break;
    case FX_MAGICBLAST:
        m_szTexName = L"Proto_FX_MagicblastTexture";
        m_iMaxFrame = 7;
        m_fFrameSpeed = 10.f;
        break;
    case FX_BULLET:
        m_szTexName = L"Proto_Effect_Bullet";
        m_iMaxFrame = 4;
        m_fFrameSpeed = 10.f;
        break;
    case FX_ROCKET:
        Engine::Get_SoundMgr()->Play_Sound(L"Explode_Rocket.wav", EFFECT, 0.27f);
        m_szTexName = L"Proto_Effect_Explosion";
        m_iMaxFrame = 9;
        m_fFrameSpeed = 10.f;
        break;
    case FX_BLOOD_RED://피격이펙트 둘 중 하나 랜덤 생성
    {
        if (5 <= rand() % 10)
        {
            m_szTexName = L"Proto_FX_Blood_Red1Texture";
        }
        else
        {
            m_szTexName = L"Proto_FX_Blood_Red2Texture";
        }
        m_iMaxFrame = 8;
        m_fFrameSpeed = 10.f;
    }
    break;
    case FX_BLOOD_YELLOW:
        m_szTexName = L"Proto_FX_Blood_YellowTexture";
        m_iMaxFrame = 8;
        m_fFrameSpeed = 10.f;
        break;
    case FX_BLOOD_GREEN:
        m_szTexName = L"Proto_FX_Blood_GreenTexture";
        m_iMaxFrame = 8;
        m_fFrameSpeed = 10.f;
        break;
    case FX_BLOOD_BLACK:
        m_szTexName = L"Proto_FX_Blood_BlackTexture";
        m_iMaxFrame = 8;
        m_fFrameSpeed = 10.f;
        break;
    default:
        break;
    }
}