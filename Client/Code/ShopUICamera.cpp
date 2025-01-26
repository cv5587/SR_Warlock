#include "stdafx.h"
#include "ShopUICamera.h"

#include "Export_System.h"

CShopUICamera::CShopUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer)
    :CUICamera(pGraphicDev), m_pPlayer(pPlayer), m_bShowShop(false), m_eSelectedWeapon(CLAYMORE), m_fFrame(0.f)
{
}

CShopUICamera::CShopUICamera(const CShopUICamera& rhs)
    :CUICamera(rhs), m_bShowShop(rhs.m_bShowShop), m_eSelectedWeapon(rhs.m_eSelectedWeapon), m_fFrame(0.f)
{
}

CShopUICamera::~CShopUICamera()
{
}

HRESULT CShopUICamera::Ready_GameObject(const _float& fWidth, const _float& fHeight,
    const _float& fPosX, const _float& fPosY)
{
    for (int i = 0; i < WEAPON_END; i++)
    {
        m_bAlreadyGet[i] = false;
    }
    for (int i = 0; i < sizeof(m_pTransformCom) / sizeof(m_pTransformCom[0]); i++)
    {
        m_pTransformCom[i] = nullptr;
    }
    Engine::CUICamera::Ready_UICamera();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    // 우선순위 설정
    m_iSortOrder = 1000;
    for (int i = 0; i < sizeof(m_pTransformCom) / sizeof(m_pTransformCom[0]); i++)
    {
        m_pTransformCom[i]->m_vScale.x = fWidth * 0.5f;
        m_pTransformCom[i]->m_vScale.y = fHeight * 0.5f;
        m_pTransformCom[i]->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));
    }
    return S_OK;
}

_int CShopUICamera::Update_GameObject(const _float& fTimeDelta)
{
    if (Engine::Get_DIKeyState(DIK_X) && m_fFrame >= 30.f)
    {
       m_bShowShop = !m_bShowShop;
       m_fFrame = 0.f;
    }
    m_fFrame += 90.f * fTimeDelta;

    GetCursorPos(&m_ptMouse);
    ScreenToClient(g_hWnd, &m_ptMouse);

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

void CShopUICamera::LateUpdate_GameObject(const float& fTimeDelta)
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

void CShopUICamera::Render_GameObject()
{
    if (!m_bShowShop) return;
    Engine::CUICamera::Render_GameObject();

    for (int i = 0; i < sizeof(m_pTransformCom) / sizeof(m_pTransformCom[0]); i++)
    {
        int iTexNum(0);
        switch (i)
        {
        case 0:
            iTexNum = 0;
            break;
        case 1:
            iTexNum = Render_Upgrade();
            break;
        case 2:
            iTexNum = Render_ArrowL();
            break;
        case 3:
            iTexNum = Render_ArrowR();
            break;
        case 4:
            iTexNum = Render_Exit();
            break;
        case 5:
            iTexNum = Render_Weapon(MINIGUN, 195.6f, 81.6f, 257.f, 137.f);
            break;
        case 6:
            iTexNum = -1;
            break;
        case 7:
            iTexNum = Render_Weapon(SHOTGUN, 195.6f, 81.6f, 462.f, 137.f);
            break;
        case 8:
            iTexNum = Render_Weapon(REVOLVER, 195.6f, 81.6f, 257.f, 225.f);
            break;
        case 9:
            iTexNum = Render_Weapon(ROCKET_LAUNCHER, 195.6f, 81.6f, 462.f, 225.f);
            break;
        default:
            iTexNum = -1;
            break;
        }

        // 버튼 출력 X 
        if (iTexNum == -1) continue;

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[i]->Get_WorldMatrix());
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        m_pTextureCom[0]->Set_Texture(iTexNum);
        m_pBufferCom->Render_Buffer();
    }

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    // 마우스 좌표 확인용
    /*_tchar x[5];
    _tchar y[5];
    _itow_s(m_ptMouse.x, x, 10);
    _itow_s(m_ptMouse.y, y, 10);
    Engine::Render_Font(L"Font_Bullet", x, &_vec2(300.f, 500.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    Engine::Render_Font(L"Font_Bullet", y, &_vec2(660.f, 500.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));*/

    _tchar iExp[5];
    _itow_s(m_pPlayer->Get_Exp(), iExp, 10);
    Engine::Render_Font(L"Font_Bullet", iExp, &_vec2(600.f, 88.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

int CShopUICamera::Render_Upgrade()
{
    _float fScaleX(214.8f), fScaleY(43.2f);
    _float fPosX = 690.f;
    _float fPosY = 362.7f;
    m_pTransformCom[1]->m_vScale.x = fScaleX * 0.5f;
    m_pTransformCom[1]->m_vScale.y = fScaleY * 0.5f;
    m_pTransformCom[1]->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

    // 마우스가 버튼 안에 들어왔을 때
    if (m_ptMouse.x > fPosX - fScaleX * 0.5f && m_ptMouse.x < fPosX + fScaleX * 0.5f &&
        m_ptMouse.y > fPosY - fScaleY * 0.5f && m_ptMouse.y < fPosY + fScaleY * 0.5f)
    {
        if (!m_bAlreadyGet[m_eSelectedWeapon] && Get_DIMouseState(DIM_LB))
        {
            int iWeaponPrice = m_pPlayer->Get_Weapon(m_eSelectedWeapon)->Get_Upgrade_Cost();
            if (m_pPlayer->Get_Exp() >= iWeaponPrice && !m_pPlayer->Get_Weapon(m_eSelectedWeapon)->Get_Upgrade())
            {
                Engine::Get_SoundMgr()->Play_Sound(L"Upgrade_Weapon.wav", PROPSOUND, 0.5f);
                m_pPlayer->Get_Weapon(m_eSelectedWeapon)->Upgrade_Weapon();
                m_pPlayer->Add_Exp(-iWeaponPrice);
                m_bAlreadyGet[m_eSelectedWeapon] = true;
            }
            else
            {
                Engine::Get_SoundMgr()->Play_Sound(L"Cannot_Buy.wav", PROPSOUND, 0.5f, false);
            }
            return 3;
        }
        else return 2;
    }
    return 1;
}

int CShopUICamera::Render_ArrowL()
{
    _float fScaleX(64.8f), fScaleY(45.6f);
    _float fPosX = 615.;
    _float fPosY = 429.f;
    m_pTransformCom[2]->m_vScale.x = fScaleX * 0.5f;
    m_pTransformCom[2]->m_vScale.y = fScaleY * 0.5f;
    m_pTransformCom[2]->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

    // 마우스가 버튼 안에 들어왔을 때
    if (m_ptMouse.x > fPosX - fScaleX * 0.5f && m_ptMouse.x < fPosX + fScaleX * 0.5f &&
        m_ptMouse.y > fPosY - fScaleY * 0.5f && m_ptMouse.y < fPosY + fScaleY * 0.5f)
    {
        if (Get_DIMouseState(DIM_LB))
        {
            Engine::Get_SoundMgr()->Play_Sound(L"Click.wav", PROPSOUND, 0.5f, false);
            return 5;
        }
        else return 4;
    }
    return -1;
}

int CShopUICamera::Render_ArrowR()
{
    _float fScaleX(64.8f), fScaleY(45.6f);
    _float fPosX = 689.f;
    _float fPosY = 429.f;
    m_pTransformCom[3]->m_vScale.x = fScaleX * 0.5f;
    m_pTransformCom[3]->m_vScale.y = fScaleY * 0.5f;
    m_pTransformCom[3]->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

    // 마우스가 버튼 안에 들어왔을 때
    if (m_ptMouse.x > fPosX - fScaleX * 0.5f && m_ptMouse.x < fPosX + fScaleX * 0.5f &&
        m_ptMouse.y > fPosY - fScaleY * 0.5f && m_ptMouse.y < fPosY + fScaleY * 0.5f)
    {
        if (Get_DIMouseState(DIM_LB))
        {
            Engine::Get_SoundMgr()->Play_Sound(L"Click.wav", PROPSOUND, 0.5f, false);
            return 7;
        }
        else return 6;
    }
    return -1;
}

int CShopUICamera::Render_Weapon(WEAPON eWeapon, _float fScaleX, _float fScaleY, _float fPosX, _float fPosY)
{
    //UI 틀 출력
    _int iTransformNum = (_int)eWeapon + 4;
    _int iUINum;
    m_pTransformCom[iTransformNum]->m_vScale.x = fScaleX * 0.5f;
    m_pTransformCom[iTransformNum]->m_vScale.y = fScaleY * 0.5f;
    m_pTransformCom[iTransformNum]->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

    if (m_bAlreadyGet[eWeapon])
    {
        iUINum = 11;
    }
    // 마우스가 버튼 안에 들어왔을 때
    else if (m_ptMouse.x > fPosX - fScaleX * 0.5f && m_ptMouse.x < fPosX + fScaleX * 0.5f &&
        m_ptMouse.y > fPosY - fScaleY * 0.5f && m_ptMouse.y < fPosY + fScaleY * 0.5f)
    {
        if (Get_DIMouseState(DIM_LB))
        {
            iUINum = 8;
            Engine::Get_SoundMgr()->Play_Sound(L"Click.wav", PROPSOUND, 0.5f, false);
            m_eSelectedWeapon = eWeapon;
        }
        else iUINum = 9;
    }
    else iUINum = 8;
    if (iUINum != -1)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[iTransformNum]->Get_WorldMatrix());
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        m_pTextureCom[0]->Set_Texture(iUINum);
        m_pBufferCom->Render_Buffer();
    }

    // 선택된 상태
    if (m_eSelectedWeapon == eWeapon && !m_bAlreadyGet[eWeapon])
    {
        m_pTextureCom[0]->Set_Texture(10);
        m_pBufferCom->Render_Buffer();
    }

    // 무기 아이콘 출력
    fScaleX = 120.f;
    fScaleY = 60.f;
    fPosX -= 32.f;
    fPosY -= 12.f;
    m_pTransformCom[iTransformNum + 10]->m_vScale.x = fScaleX * 0.5f;
    m_pTransformCom[iTransformNum + 10]->m_vScale.y = fScaleY * 0.5f;
    m_pTransformCom[iTransformNum + 10]->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[iTransformNum + 10]->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pTextureCom[1]->Set_Texture((_int)eWeapon);
    m_pBufferCom->Render_Buffer();

    // Price
    _tchar price[3];
    _itow_s(m_pPlayer->Get_Weapon(eWeapon)->Get_Upgrade_Cost(), price, 10);
    Engine::Render_Font(L"Font_Health", price, &_vec2(fPosX + 65.f, fPosY), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    // Description
    switch (m_eSelectedWeapon)
    {
    case MINIGUN:
        Engine::Render_Font(L"Font_Health", L"Upgrade Minigun.", &_vec2(590.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        break;
    case SHOTGUN:
        Engine::Render_Font(L"Font_Health", L"Upgrade Shotgun.", &_vec2(590.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        break;
    case REVOLVER:
        Engine::Render_Font(L"Font_Health", L"Upgrade Revolver.", &_vec2(590.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        break;
    case ROCKET_LAUNCHER:
        Engine::Render_Font(L"Font_Health", L"Upgrade Rocket_Lau\nncher.", &_vec2(590.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        break;
    }

    return -1;
}

int CShopUICamera::Render_Exit()
{
    _float fScaleX(48.f), fScaleY(34.8f);
    _float fPosX = 769.f;
    _float fPosY = 430.f;
    m_pTransformCom[4]->m_vScale.x = fScaleX * 0.5f;
    m_pTransformCom[4]->m_vScale.y = fScaleY * 0.5f;
    m_pTransformCom[4]->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

    // 마우스가 버튼 안에 들어왔을 때
    if (m_ptMouse.x > fPosX - fScaleX * 0.5f && m_ptMouse.x < fPosX + fScaleX * 0.5f &&
        m_ptMouse.y > fPosY - fScaleY * 0.5f && m_ptMouse.y < fPosY + fScaleY * 0.5f)
    {
        if (Get_DIMouseState(DIM_LB))
        {
            m_bShowShop = !m_bShowShop;
            Engine::Get_SoundMgr()->Play_Sound(L"Click.wav", PROPSOUND, 0.5f, false);
            m_pPlayer->Set_ShopOn(false);
            return -1;
        }
    }
    return 12;
}

HRESULT CShopUICamera::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

    for (int i = 0; i < 20; i++)
    {
        pComponent = m_pTransformCom[i] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
        NULL_CHECK_RETURN(pComponent, E_FAIL);
        m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
    }

    pComponent = m_pTextureCom[0] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shop"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_Shop", pComponent });

    pComponent = m_pTextureCom[1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIWeapon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Proto_UIWeapon", pComponent });

    return S_OK;
}

CShopUICamera* CShopUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fWidth,
    const _float& fHeight, const _float& fPosX, const _float& fPosY, CPlayer*& pPlayer)
{
    CShopUICamera* pInstance = DBG_NEW CShopUICamera(pGraphicDev, pPlayer);

    if (FAILED(pInstance->Ready_GameObject(fWidth, fHeight, fPosX, fPosY)))
    {
        Safe_Release(pInstance);
        MSG_BOX("ShopUICamera Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CShopUICamera::Free()
{
    for (int i = 0; i < 20; ++i)
    {
        Safe_Release(m_pTransformCom[i]);
    }
    for (int i = 0; i < 2; ++i)
    {
        Safe_Release(m_pTextureCom[i]);
    }
    Engine::CUICamera::Free();
}