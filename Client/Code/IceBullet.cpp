#include "stdafx.h"
#include "..\Header\IceBullet.h"

#include "Export_Utility.h"

CIceBullet::CIceBullet(LPDIRECT3DDEVICE9 pGraphicDev, _int iTexNum, _vec3 vStart)
    : Engine::CGameObject(pGraphicDev), m_bDead(false), m_bInit(true), m_vStart(vStart), m_iTexNum(iTexNum)
{
}

CIceBullet::CIceBullet(const CIceBullet& rhs)
    : Engine::CGameObject(rhs), m_bDead(rhs.m_bDead), m_bInit(rhs.m_bInit), m_vStart(rhs.m_vStart), m_iTexNum(rhs.m_iTexNum)
{
    Ready_GameObject();
}

CIceBullet::~CIceBullet()
{
}

HRESULT CIceBullet::Ready_GameObject()
{
    TexNamer();
    m_pPlayer = dynamic_cast<CPlayer*>(Get_GameObject(L"GameLogic", L"Player"));
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    Push_CollisionMgr(OT_NONPASSPROP);
    return S_OK;
}

Engine::_int CIceBullet::Update_GameObject(const _float& fTimeDelta)
{
    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;
    if (m_iMaxFrame <= (_uint)(m_fFrame))
    {
        //애니메이션 한바퀴 돌고 삭제
        m_bDead = true;
    }

    if (!m_bDead) //안죽었으면 초당 데미지 줌
    {
        if (m_fAtkRng > DistanceToPlayer(m_pPlayer))
        {
            m_fAtkTimer += 10 * fTimeDelta;
            if (3.f < m_fAtkTimer)
            {
                m_fAtkTimer = 0.f;
                m_pPlayer->Add_Health(-m_iDamage);
            }
        }
    }

    CGameObject::Update_GameObject(fTimeDelta);

    m_pTransformCom->Set_Pos(m_vStart);
    m_pTransformCom->Update_Component(fTimeDelta);


    return 0;
}

void CIceBullet::LateUpdate_GameObject(const float& fTimeDelta)
{

    __super::LateUpdate_GameObject(fTimeDelta);


}

void CIceBullet::Render_GameObject()
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

    m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}


HRESULT CIceBullet::Add_Component()
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
    //왜 다이나믹으로 바꾸면 출력이 안되는가

    return S_OK;
}

_float CIceBullet::DistanceToPlayer(CPlayer* pPlayer)
{
    _vec3 vPos;
    _matrix* vWorld;

    vWorld = m_pTransformCom->Get_WorldMatrix();

    vPos.x = vWorld->m[3][0];
    vPos.y = vWorld->m[3][1];
    vPos.z = vWorld->m[3][2];

    return D3DXVec3Length(&(pPlayer->Get_Player_Eye() - vPos));
}

void CIceBullet::TexNamer()
{
    switch (m_iTexNum)
    {
    case 0:
        m_szTexName = L"Proto_IceBulletATexture";
        m_iMaxFrame = 18;
        m_fFrameSpeed = 10.f;
        m_iDamage = 10;
        break;
    case 1:
        m_szTexName = L"Proto_IceBulletBTexture";
        m_iMaxFrame = 16;
        m_fFrameSpeed = 10.f;
        m_iDamage = 15;
        break;
    case 2:
        m_szTexName = L"Proto_IceBulletCTexture";
        m_iMaxFrame = 18;
        m_fFrameSpeed = 10.f;
        m_iDamage = 20;
        break;
    case 3:
        m_szTexName = L"Proto_FireTexture";
        m_iMaxFrame = 7;
        m_fFrameSpeed = 10.f;
        m_iDamage = 1;
        break;
    case 4:
        m_szTexName = L"Proto_Boss2SpikeTextureA";
        m_iMaxFrame = 9;
        m_fFrameSpeed = 10.f;
        m_iDamage = 10;
        m_fAtkRng = 3.f;
        break;
    case 5:
        m_szTexName = L"Proto_Boss2SpikeTextureB";
        m_iMaxFrame = 11;
        m_fFrameSpeed = 10.f;
        m_iDamage = 10;
        m_fAtkRng = 3.f;
        break;
    case 6:
        m_szTexName = L"Proto_Boss2SpikeTextureC";
        m_iMaxFrame = 18;
        m_fFrameSpeed = 10.f;
        m_iDamage = 12;
        m_fAtkRng = 3.f;
        break;
    case 7:
        m_szTexName = L"Proto_Thingy_SpikeTexture";
        m_iMaxFrame = 20;
        m_fFrameSpeed = 10.f;
        m_iDamage = 10;
        break;
    default:
        break;
    }
}



CIceBullet* CIceBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iTexNum, _vec3 vStart)
{
    CIceBullet* pInstance = DBG_NEW CIceBullet(pGraphicDev, iTexNum, vStart);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("IceBullet Create Failed");
        return nullptr;
    }
    return pInstance;
}

void CIceBullet::Free()
{
    __super::Free();
}