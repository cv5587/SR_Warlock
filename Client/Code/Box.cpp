#include "stdafx.h"
#include "..\Header\Box.h"
#include "Item.h"
#include "Explosion.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID)
    : CBreakableProp(pGraphicDev, eBoxID)
{
}

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID, _bool isFront, _float yAngle)
    : CBreakableProp(pGraphicDev, eBoxID), m_fYAngle(yAngle), m_bIsZBox(FALSE), m_bisFront(isFront)
{
}

CBox::CBox(const CBox& rhs)
    : CBreakableProp(rhs), m_fYAngle(rhs.m_fYAngle), m_bIsZBox(rhs.m_bIsZBox), m_bisFront(rhs.m_bisFront)
{

}

CBox::~CBox()
{
}

HRESULT CBox::Ready_GameObject()
{
    TexNamer();
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Pos(0.f, 1.f, 3.f);
    m_vPrePos = m_pTransformCom->Get_Pos();
    m_fFrame = 0.f; //秦拎具 面倒贸府凳
    m_eObjType = OT_NONPASSPROP;
    _vec3 vPos = m_pTransformCom->Get_Pos();
    switch (m_eBoxID)
    {
    case BOX_NORMAL:
        m_pTransformCom->Set_Pos({ vPos.x, 1.285f, vPos.z });
        m_pTransformCom->Set_Scale({ 0.3f, 0.3f, 0.3f });
        m_lowerBound = vPos - D3DXVECTOR3(0.5f, 0.5f, 0.5f);
        m_upperBound = vPos + D3DXVECTOR3(0.5f, 0.5f, 0.5f);


        //Push_CollisionMgr(OT_NONPASSPROP,0.8f);
        break;
    case BOX_EXPL:
    {
        m_pTransformCom->Set_Pos({ vPos.x, 1.285f, vPos.z });
        m_pTransformCom->Set_Scale({ 0.3f, 0.3f, 0.3f });
        m_lowerBound = vPos - D3DXVECTOR3(0.5f, 0.5f, 0.5f);
        m_upperBound = vPos + D3DXVECTOR3(0.5f, 0.5f, 0.5f);

        //Push_CollisionMgr(OT_NONPASSPROP,0.8f);
    }
    break;
    case PILLAR:
        m_lowerBound = { m_vPrePos.x - 0.15f, m_vPrePos.y - 1.f, m_vPrePos.z - 0.15f };
        m_upperBound = { m_vPrePos.x + 0.15f, m_vPrePos.y + 1.f, m_vPrePos.z + 0.15f };
        //Push_CollisionMgr(OT_NONPASSPROP, lowerBound, upperBound);
        break;
    case BROKEN_WALL:
    case BROKEN_DOOR:
    {
        m_bIsPlane = true;
        m_pTransformCom->Set_Angle({ 0.f,D3DXToRadian(m_fYAngle),0.f });
        constexpr float EPSILON = D3DXToRadian(45.f);
        constexpr float PI_HALF = D3DX_PI * 0.5f;

        float fAbsYAngle = abs(D3DXToRadian(m_fYAngle));

        if (fAbsYAngle >= D3DXToRadian(360.f))
        {
            fAbsYAngle -= D3DXToRadian(360.f);
        }

        if ((fAbsYAngle < EPSILON && 0.f <= fAbsYAngle) ||
            (fAbsYAngle < (D3DX_PI + EPSILON) && (D3DX_PI - EPSILON) < fAbsYAngle) ||
            (fAbsYAngle < 2 * D3DX_PI && (D3DX_PI + PI_HALF + EPSILON) <= fAbsYAngle))
        {
            if (!m_bisFront)
            {
                m_lowerBound = { vPos.x - 1.f, vPos.y - 1.f, vPos.z - 0.2f };
                m_upperBound = { vPos.x + 1.f, vPos.y + 1.f, vPos.z + 0.2f };
                m_bIsZBox = false;

            }
            else
            {
                m_lowerBound = { vPos.x - 1.f, vPos.y - 1.f, vPos.z - 0.2f };
                m_upperBound = { vPos.x + 1.f, vPos.y + 1.f, vPos.z + 0.2f };
                m_bIsZBox = false;

            }
            //Push_CollisionMgr(OT_NONPASSPROP, lowerBound, upperBound);
        }
        else if ((fAbsYAngle < (PI_HALF + EPSILON) && (PI_HALF - EPSILON) < fAbsYAngle) ||
            (fAbsYAngle < (D3DX_PI + PI_HALF + EPSILON) && (D3DX_PI + PI_HALF - EPSILON) < fAbsYAngle))
        {
            if (!m_bisFront)
            {
                m_lowerBound = { vPos.x - 0.4f, vPos.y - 1.f, vPos.z - 1.f };
                m_upperBound = { vPos.x + 0.f, vPos.y + 1.f, vPos.z + 1.f };
                m_bIsZBox = true;

            }
            else
            {
                m_lowerBound = { vPos.x - 0.f, vPos.y - 1.f, vPos.z - 1.f };
                m_upperBound = { vPos.x + 0.4f, vPos.y + 1.f, vPos.z + 1.f };
                m_bIsZBox = true;

            }
            //Push_CollisionMgr(OT_NONPASSPROP, lowerBound, upperBound);
        }
    }
    break;

    default:
        break;
    }

    m_bColliderIsNone = true;

    return S_OK;
}

Engine::_int CBox::Update_GameObject(const _float& fTimeDelta)
{

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    CGameObject::Update_GameObject(fTimeDelta);


    if (m_bDead)
    {
        switch (m_eBoxID)
        {
        case BOX_NORMAL:
        {
            CGameObject* pGameObject;
            _vec3 vPos;
            m_pTransformCom->Get_Info(INFO_POS, &vPos);
            ITEMTYPE it = static_cast<ITEMTYPE>((rand() % 3) + 1);
            pGameObject = CItem::Create(m_pGraphicDev, vPos);
            NULL_CHECK_RETURN(pGameObject, E_FAIL);
            FAILED_CHECK_RETURN(Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BoxItem", pGameObject), E_FAIL);
            pGameObject->Push_CollisionMgr();
        }
        break;
        case BOX_EXPL:
        {
            CGameObject* pGameObject;
            _vec3 vPos;
            m_pTransformCom->Get_Info(INFO_POS, &vPos);
            pGameObject = CExplosion::Create(m_pGraphicDev, vPos);

            if (pGameObject != nullptr)
            {
                Engine::Get_Layer(L"GameLogic")->Add_GameObject(L"BoxExplEffect", pGameObject);
            }
        }
        break;
        }

        return OBJ_DEAD;
    }
    else
        return OBJ_NOEVENT;
}

void CBox::LateUpdate_GameObject(const float& fTimeDelta)
{
    __super::LateUpdate_GameObject(fTimeDelta);

}

void CBox::Render_GameObject()
{
    if (m_eBoxID != BROKEN_WALL && m_eBoxID != BROKEN_DOOR)
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

    }

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(_uint(floorf(m_fFrame)));

    m_pBufferCom->Render_Buffer();

}

void CBox::TexNamer()
{
    switch (m_eBoxID)
    {
    case BOX_NORMAL:
        m_szTexName = L"Proto_BoxNormalTexture";
        m_iMaxFrame = 1;
        break;
    case BOX_EXPL:
        m_szTexName = L"Proto_BoxExplTexture";
        m_iMaxFrame = 1;
        break;
    case PILLAR:
        m_szTexName = L"Proto_PillarTexture";
        m_iMaxFrame = 5;
        break;
    case BROKEN_WALL:
        m_szTexName = L"Proto_Broken_WallTexture";
        m_iMaxFrame = 4;
        break;
    case BROKEN_DOOR:
        m_szTexName = L"Proto_Broken_DoorTexture";
        m_iMaxFrame = 7;
        break;
    default:
        break;
    }
}

HRESULT CBox::Add_Component()
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

void CBox::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{
    OBJ_TYPE ot = pGameObject->Get_ObjType();

    switch (ot)
    {
    case OT_PLAYER:
        if (m_eBoxID == BROKEN_WALL || m_eBoxID == BROKEN_DOOR)
        {
            if (m_bIsZBox)
            {
                D3DXVECTOR3 wallNormalDir = { m_upperBound.x - m_lowerBound.x,0.f,0.f };
                D3DXVECTOR3 wallNormalizedDir;
                D3DXVec3Normalize(&wallNormalizedDir, &wallNormalDir);
                D3DXVECTOR3 objDir = pGameObject->Get_TransformCom_Pointer()->Get_Pos() - pGameObject->Get_PrePos();
                float dot = D3DXVec3Dot(&wallNormalizedDir, &objDir);
                pGameObject->Move_Pos(-dot * wallNormalizedDir);
            }
            else
            {
                D3DXVECTOR3 wallNormalDir = { 0.f,0.f,m_upperBound.z - m_lowerBound.z };
                D3DXVECTOR3 wallNormalizedDir;
                D3DXVec3Normalize(&wallNormalizedDir, &wallNormalDir);
                D3DXVECTOR3 objDir = pGameObject->Get_TransformCom_Pointer()->Get_Pos() - pGameObject->Get_PrePos();
                float dot = D3DXVec3Dot(&wallNormalizedDir, &objDir);
                pGameObject->Move_Pos(-dot * wallNormalizedDir);
            }
        }
        else
        {
            CGameObject::Action_On_Collision(pGameObject, fTimeDelta);


        }
        break;
    case OT_PLAYER_BULLET:
    {
        if (_int(m_fFrame) < m_iMaxFrame)
        {
            m_fFrame += 1.f;
            switch (m_eBoxID)
            {
            case BOX_NORMAL:
            case BOX_EXPL:
                Engine::Get_SoundMgr()->Play_Sound(L"barrel_break.wav", PROPSOUND, 0.5f);
                break;
            case PILLAR:
                Engine::Get_SoundMgr()->Play_Sound(L"break_pillar.wav", PROPSOUND, 0.5f);
                break;
            case BROKEN_WALL:
                Engine::Get_SoundMgr()->Play_Sound(L"break_pillar.wav", PROPSOUND, 0.5f);
                break;
            case BROKEN_DOOR:
                Engine::Get_SoundMgr()->Play_Sound(L"break_pillar.wav", PROPSOUND, 0.5f);
                break;
            }
        }
        else
        {
            m_bDead = true;
        }
    }
    break;
    default:
        break;
    }


}

void CBox::Action_On_Collision(vector<CLine*> pvecLines)
{
}

CBox* CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID)
{
    CBox* pInstance = DBG_NEW CBox(pGraphicDev, eBoxID);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Prop Create Failed");
        return nullptr;
    }

    return pInstance;
}

CBox* CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID, _bool isFront, _float yAngle)
{
    CBox* pInstance = DBG_NEW CBox(pGraphicDev, eBoxID, isFront, yAngle);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Prop Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CBox::Free()
{
    __super::Free();
}