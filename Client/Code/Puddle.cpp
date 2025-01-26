#include "stdafx.h"
#include "Puddle.h"

#include "Export_System.h"
#include "Export_Utility.h"


CPuddle::CPuddle(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
    : CEffect(pGraphicDev, m_eFXID, vPos),  m_fAge(0)
{
    m_vStart = vPos;
}

CPuddle::CPuddle(const CPuddle& rhs)
    : CEffect(rhs),  m_fAge(rhs.m_fAge)
{

}

CPuddle::~CPuddle()
{
}

HRESULT CPuddle::Ready_GameObject()
{
    TexNamer();
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Pos(m_vStart);
    //m_pTransformCom->Set_Scale({ 1.5f, 1.5f, 1.5f });
    m_pTransformCom->Set_Angle({ 0,0,0 });

    m_vPrePos = m_pTransformCom->Get_Pos();
    m_fRadius = 0.05f;
    Push_CollisionMgr(OT_PASSPROP, m_fRadius);
    m_fFrame += GetRandomInt(0, m_iMaxFrame);

    return S_OK;
}

Engine::_int CPuddle::Update_GameObject(const _float& fTimeDelta)
{


    m_fAge += fTimeDelta;

    if (m_fAge > 10.f)
        return OBJ_DEAD;

    CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);


    return 0;
}
_uint CPuddle::GetRandomInt(_uint lowBound, _uint highBound)
{
    if (lowBound >= highBound)
    {
        return lowBound;
    }
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist(lowBound, highBound);

    return dist(gen);
}
void CPuddle::LateUpdate_GameObject(const float& fTimeDelta)
{
    __super::LateUpdate_GameObject(fTimeDelta);

}

void CPuddle::Render_GameObject()
{
    //_matrix   matWorld, matView, matBill;

    //m_pTransformCom->Get_WorldMatrix(&matWorld);
    //m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    //D3DXMatrixIdentity(&matBill);

    //matBill._11 = matView._11;
    //matBill._13 = matView._13;
    //matBill._31 = matView._31;
    //matBill._33 = matView._33;

    //D3DXMatrixInverse(&matBill, NULL, &matBill);
    //m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture((_uint)(m_fFrame) );

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CPuddle::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    OBJ_TYPE ot = pGameObject->Get_ObjType();

    switch (ot)
    {
    case OT_PLAYER:

        break;
    case OT_MONSTER:
    
        break;
    case OT_WALL:
    {

    }
        break;
    case OT_FLOOR:
    {
    }
         break;
    default:
        break;
    }


}

HRESULT CPuddle::Add_Component()
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

CPuddle* CPuddle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
    CPuddle* pInstance = new CPuddle(pGraphicDev, vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CPuddle::Free()
{
    CEffect::Free();
}

void CPuddle::TexNamer()
{
    m_szTexName = L"Proto_Puddle_Texture";

    m_iMaxFrame = 3;

}