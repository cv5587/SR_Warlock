#include "stdafx.h"
#include "..\Header\Explosion.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Player.h"
#include "Monster.h"
#include "Fire.h"
CExplosion::CExplosion(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
    : CEffect(pGraphicDev, m_eFXID, vPos), m_bDmged(false), m_iFireCount(0)
{
    m_vStart = vPos;
}

CExplosion::CExplosion(const CExplosion& rhs)
    : CEffect(rhs)
{

}

CExplosion::~CExplosion()
{
}

HRESULT CExplosion::Ready_GameObject()
{
    TexNamer();
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Pos(m_vStart);
    m_pTransformCom->Set_Scale({ 1.5f, 1.5f, 1.5f });

    m_vPrePos = m_pTransformCom->Get_Pos();
    m_fRadius = 5.f;
    Push_CollisionMgr(OT_BOMB, m_fRadius);

    return S_OK;
}

Engine::_int CExplosion::Update_GameObject(const _float& fTimeDelta)
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
        return OBJ_DEAD;

    CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);


    return 0;
}

void CExplosion::LateUpdate_GameObject(const float& fTimeDelta)
{
    __super::LateUpdate_GameObject(fTimeDelta);


    __super::Compute_ViewZ(m_pTransformCom->Get_Pos());

}

void CExplosion::Render_GameObject()
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
    //m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

    m_pBufferCom->Render_Buffer();

    //m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CExplosion::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    if (m_bDmged) return;
    OBJ_TYPE ot = pGameObject->Get_ObjType();

    switch (ot)
    {
    case OT_PLAYER:
        dynamic_cast<CPlayer*>(pGameObject)->Add_Health(-20);
        m_bDmged = true;
        break;
    case OT_MONSTER:
    {
        dynamic_cast<CMonster*>(pGameObject)->Take_Dmg(100);
        m_bDmged = true;
    }
        break;
    case OT_WALL:
    {

    }
        break;
    case OT_FLOOR:
    {}
    break;
    default:
        break;
    }

    
}

void CExplosion::Action_On_Collision(vector<CLine*> pvecLines)
{
    if (m_iFireCount < 1)
    {
        CGameObject* pFire;
        _vec3 vPos;
        pFire = CFire::Create(m_pGraphicDev);

        if (pFire != nullptr)
        {
            dynamic_cast<CTransform*>(pFire->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(m_pTransformCom->Get_Pos() + _vec3{_float(rand() % 5), 0.f, _float(rand() % 5) });
            Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"ExplosionFire", pFire);
            ++m_iFireCount;
        }
    }
    
}

HRESULT CExplosion::Add_Component()
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

CExplosion* CExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
    CExplosion* pInstance = new CExplosion(pGraphicDev, vPos);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Effect Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CExplosion::Free()
{
    CEffect::Free();
}

void CExplosion::TexNamer()
{
     
    int i = rand() % 15;
    if (5 > i)
    {
        m_szTexName = L"Proto_FX_Expl_1Texture";

    }
    else if (10 > i)
    {
        m_szTexName = L"Proto_FX_Expl_2Texture";
    }
    else
    {
        m_szTexName = L"Proto_FX_Expl_3Texture";

    }

    wstring szSoundFile = L"";
    int s = rand() % 20;
    if (5 > s)
    {
        szSoundFile = L"explosion_0.wav";
    }
    else if (10 > s)
    {
        szSoundFile = L"explosion_1.wav";
    }
    else if (15 > s)
    {
        szSoundFile = L"explosion_2.wav";

    }
    else
    {
        szSoundFile = L"explosion_3.wav";
    }

    Engine::Get_SoundMgr()->Play_Sound(szSoundFile, MONSTERSOUND, 0.56f);

    m_iMaxFrame = 13;
    m_fFrameSpeed = 10.f;
     
}