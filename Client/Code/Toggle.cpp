#include "stdafx.h"
#include "..\Header\Toggle.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Player.h"
#include "Door.h"

CToggle::CToggle(LPDIRECT3DDEVICE9 pGraphicDev, TOGGLEID eToggleID)
    : CInteractiveProp(pGraphicDev), m_bToggleOn(false), m_eToggleID(eToggleID)
{
}

CToggle::CToggle(const CToggle& rhs)
    : CInteractiveProp(rhs)
{
}

CToggle::~CToggle()
{
}

HRESULT CToggle::Ready_GameObject()
{
    TexNamer();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->Set_Pos(10.f, 10.f, 10.f);
    m_pTransformCom->Set_Pos(0.5f, 0.5f, 0.5f);
    m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 0.5f });
    m_bInit = false;
    m_fFrame = 0.f;

    m_bIsPlane = true;
    m_vPrePos = m_pTransformCom->Get_Pos();

    return S_OK;

}

HRESULT CToggle::Ready_GameObject(D3DXVECTOR3 vPos, float fYAngle)
{
    TexNamer();
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_bInit = false;
    m_fFrame = 0.f;
    m_bIsPlane = true;

    m_vPrePos = vPos;
    m_pTransformCom->Set_Pos(vPos);
    m_pTransformCom->Set_Angle({ 0.f, fYAngle, 0.f });
    m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 0.5f });
    constexpr float EPSILON = D3DXToRadian(45.f);
    constexpr float PI_HALF = D3DX_PI * 0.5f;

    m_eObjType = OT_NONPASSPROP;

    float fAbsYAngle = abs(fYAngle);

    if (fAbsYAngle >= D3DXToRadian(360.f))
    {
        fAbsYAngle -= D3DXToRadian(360.f);
    }

    if ((fAbsYAngle < EPSILON && 0.f <= fAbsYAngle) ||
        (fAbsYAngle < (D3DX_PI + EPSILON) && (D3DX_PI - EPSILON) < fAbsYAngle) ||
        (fAbsYAngle < 2 * D3DX_PI && (D3DX_PI + PI_HALF + EPSILON) <= fAbsYAngle))
    {

        m_lowerBound = { vPos.x - 1.5f, vPos.y - 1.f, vPos.z - 0.2f };
        m_upperBound = { vPos.x + 1.5f, vPos.y + 1.f, vPos.z + 0.2f };
        m_bIsZToggle = false;
        //Push_CollisionMgr(OT_NONPASSPROP, lowerBound, upperBound);
    }
    else if ((fAbsYAngle < (PI_HALF + EPSILON) && (PI_HALF - EPSILON) < fAbsYAngle) ||
        (fAbsYAngle < (D3DX_PI + PI_HALF + EPSILON) && (D3DX_PI + PI_HALF - EPSILON) < fAbsYAngle))
    {
        m_lowerBound = { vPos.x - 0.2f, vPos.y - 1.f, vPos.z - 1.5f };
        m_upperBound = { vPos.x + 0.2f, vPos.y + 1.f, vPos.z + 1.5f };
        m_bIsZToggle = true;
        //Push_CollisionMgr(OT_NONPASSPROP, lowerBound, upperBound);
    }
    else
    {
        MSG_BOX("Invalid YAngle");
        return E_FAIL;
    }

    m_bColliderIsNone = true;

    return S_OK;
}

Engine::_int CToggle::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bInit)
    {
        m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
        m_bInit = true;
    }

    Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

    CGameObject::Update_GameObject(fTimeDelta);

    Detect_Player();

    if (m_bToggleOn && (Engine::Get_DIKeyState(DIK_SPACE) & 0x80))
    {
        switch (m_eToggleID)
        {
        case TOG_NORMAL:
            //dynamic_cast<CDoor*>(Engine::Get_GameObject(L"GameLogic", L"Doors"))->Set_Elevate();
            break;
        case TOG_RED:
            //DOOR_RED
        {
            if (!m_pPlayer->Get_Key(DOOR_RED)) //플레이어 키 여부 확인
            {
                m_pPlayer->Set_Key(DOOR_RED);
            }
        }
        break;
        case TOG_YELLOW:
        {
            if (!m_pPlayer->Get_Key(DOOR_YELLOW)) //플레이어 키 여부 확인
            {
                m_pPlayer->Set_Key(DOOR_YELLOW);
            }
        }
        break;
        case TOG_BLUE:
        {
            if (!m_pPlayer->Get_Key(DOOR_BLUE)) //플레이어 키 여부 확인
            {
                m_pPlayer->Set_Key(DOOR_BLUE);
                m_fFrame = 1.f;
            }
        }
        break;
        }
        m_fFrame = 1.f; //누른 프레임으로 바꿈
        Engine::Get_SoundMgr()->Play_Sound(L"Toggle_Switch.wav", PROPSOUND, 0.5f);
    }

    return 0;
}

void CToggle::LateUpdate_GameObject(const float& fTimeDelta)
{

    __super::LateUpdate_GameObject(fTimeDelta);

    _vec3   vPos = m_pTransformCom->Get_Pos();
    __super::Compute_ViewZ(vPos);

}

void CToggle::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture((_uint)m_fFrame);

    m_pBufferCom->Render_Buffer();

}

HRESULT CToggle::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ m_szTexName.c_str() , pComponent });


    pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

    return S_OK;
}

void CToggle::TexNamer()
{
    //ToggleID 값 따라 다른 텍스처 출력

    switch (m_eToggleID)
    {
    case TOG_NORMAL:
        m_szTexName = L"Proto_ToggleNormalTexture";
        m_eDoorID = DOOR_GREY;
        break;
    case TOG_RED:
        m_szTexName = L"Proto_ToggleRedTexture";
        m_eDoorID = DOOR_RED;
        break;
    case TOG_YELLOW:
        m_szTexName = L"Proto_ToggleYellowTexture";
        m_eDoorID = DOOR_YELLOW;
        break;
    case TOG_BLUE:
        m_szTexName = L"Proto_ToggleBlueTexture";
        m_eDoorID = DOOR_BLUE;
        break;
    default:
        break;
    }
}


void CToggle::Detect_Player()
{
    _vec3 vPos, vPlayerPos;
    vPlayerPos = m_pPlayer->Get_Player_Eye();
    m_pTransformCom->Get_Info(INFO_POS, &vPos);


    if (!m_bToggleOn && 3.f > D3DXVec3Length(&(vPlayerPos - vPos))) //test
    {
        m_bToggleOn = true;
    }
}

CToggle* CToggle::Create(LPDIRECT3DDEVICE9 pGraphicDev, TOGGLEID eTogID)
{
    CToggle* pInstance = new CToggle(pGraphicDev, eTogID);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        MSG_BOX("Door Create Failed");
        return nullptr;
    }

    return pInstance;
}

CToggle* CToggle::Create(LPDIRECT3DDEVICE9 pGraphicDev, TOGGLEID eTogID, D3DXVECTOR3 vPos, float fYAngle)
{
    CToggle* pInstance = new CToggle(pGraphicDev, eTogID);

    if (FAILED(pInstance->Ready_GameObject(vPos,fYAngle)))
    {
        Safe_Release(pInstance);
        MSG_BOX("Door Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CToggle::Action_On_Collision(CGameObject* pGameObject, float fTimeDelta)
{


    if (m_bIsZToggle)
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

    pGameObject->LateUpdate_GameObject(fTimeDelta);
}

void CToggle::Action_On_Collision(vector<CLine*> pvecLines)
{
}

void CToggle::Free()
{
    __super::Free();
}