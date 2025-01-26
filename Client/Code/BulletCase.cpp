#include "stdafx.h"
#include "..\Header\BulletCase.h"
#include "Export_Utility.h"

CBulletCase::CBulletCase(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_fFrame(0.f)
{
}

CBulletCase::CBulletCase(const CBulletCase& rhs)
    : Engine::CGameObject(rhs), m_fFrame(0.f)
{
    Ready_GameObject();
}

CBulletCase::~CBulletCase()
{
    Free();
}

HRESULT CBulletCase::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_fSpeed = (rand() % 300 + 200) / 100.f;
    m_pTransformCom->Rotation(ROT_Z, _float(rand() % 360));

    return S_OK;
}

Engine::_int CBulletCase::Update_GameObject(const _float& fTimeDelta)
{
    m_fFrame += 90.f * fTimeDelta;
    if (m_fFrame > 90.f) return OBJ_DEAD;
    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    CGameObject::Update_GameObject(fTimeDelta);

    m_fPower -= 0.2f;
    _vec3 vMove = m_vDir + m_vUp * m_fPower;
    if (m_pTransformCom_Clone->Get_Pos().y <= 1.2f) vMove = { 0.f, 0.f, 0.f };
    Move_Pos(vMove, m_fSpeed, fTimeDelta);
    m_pTransformCom->m_vScale = { 0.15f, 0.15f, 0.15f };
    m_pTransformCom->Update_Component(fTimeDelta);

    Compute_ViewZ(m_pTransformCom->Get_Pos());

    return 0;
}

void CBulletCase::LateUpdate_GameObject(const float& fTimeDelta)
{
    __super::LateUpdate_GameObject(fTimeDelta);
    Compute_ViewZ(m_pTransformCom->Get_Pos());
}

void CBulletCase::Render_GameObject()
{
    /*_matrix   matWorld, matView, matBill;

    m_pTransformCom->Get_WorldMatrix(&matWorld);
    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;

    D3DXMatrixInverse(&matBill, NULL, &matBill);

    m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));*/
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBulletCase::Set_Pos(_vec3 vPos)
{
    m_pTransformCom->Set_Pos(vPos + m_vRight * 0.3f - m_vUp * 0.3f);
}

void CBulletCase::Set_Dir(_vec3 Dir)
{
    m_vDir = Dir;
    _vec3 vUp = { 0.f, 1.f, 0.f };
    D3DXVec3Cross(&m_vRight, &vUp, &m_vDir);
    D3DXVec3Cross(&m_vUp, &m_vDir, &m_vRight);
}


HRESULT CBulletCase::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BulletCase"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_BulletCase", pComponent });

    return S_OK;
}

CBulletCase* CBulletCase::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBulletCase* pInstance = DBG_NEW CBulletCase(pGraphicDev);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("BulletCase Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CBulletCase::Free()
{
    __super::Free();
}