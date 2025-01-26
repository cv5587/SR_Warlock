#include "stdafx.h"
#include "BarUICamera.h"

#include "Export_System.h"

CBarUICamera::CBarUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer, BARTYPE eBarType, _int iTexNum)
    :CUICamera(pGraphicDev), m_eBarType(eBarType),
    m_iTexNum(iTexNum), m_pPlayer(pPlayer)
{
}

CBarUICamera::CBarUICamera(const CBarUICamera& rhs)
    : CUICamera(rhs), m_eBarType(rhs.m_eBarType), m_iTexNum(rhs.m_iTexNum)
{
}

CBarUICamera::~CBarUICamera()
{
}

HRESULT CBarUICamera::Ready_GameObject(const _float& fWidth, const _float& fHeight, const _float& fPosX, const _float& fPosY)
{
    Engine::CUICamera::Ready_UICamera();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    // 맨 위에 출력하기 위해 값 설정
    m_iSortOrder = m_iTexNum;
    m_pTransformCom->m_vScale.x = fWidth * 0.5f;
    m_pTransformCom->m_vScale.y = fHeight * 0.5f;
    m_pTransformCom->Set_Pos(_vec3(fPosX - WINCX * 0.5f, -fPosY + WINCY * 0.5f, 0.0f));

    return S_OK;
}

_int CBarUICamera::Update_GameObject(const _float& fTimeDelta)
{
    Engine::Add_RenderGroup(RENDER_UI, this);
    Engine::CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CBarUICamera::LateUpdate_GameObject(const float& fTimeDelta)
{
    Engine::CUICamera::LateUpdate_GameObject(fTimeDelta);
}

void CBarUICamera::Render_GameObject()
{
    Engine::CUICamera::Render_GameObject();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    IDirect3DTexture9* pTexture = nullptr;
    IDirect3DTexture9* pTexture2 = nullptr;
    int iTexNum(0);
    float ratio(0.f);
    switch (m_eBarType)
    {
    case BAR_MANA:
        iTexNum = 3;
        ratio = (_float)m_pPlayer->Get_Mana() /
            max(m_pPlayer->Get_MaxMana(), 1);
        break;
    case BAR_HEALTH:
        iTexNum = 5;
        ratio = (_float)m_pPlayer->Get_Health() /
            max(m_pPlayer->Get_MaxHealth(), 1);
        break;
    case BAR_BULLET:
        iTexNum = 7;
        ratio = (_float)m_pPlayer->Get_Weapon()->Get_CurBullet() /
            max(m_pPlayer->Get_Weapon()->Get_MaxBullet(), 1);
        break;
    }
    pTexture = (IDirect3DTexture9*)m_pTextureCom->Get_Texture(iTexNum);
    pTexture2 = (IDirect3DTexture9*)m_pTextureCom->Get_Texture(iTexNum + 1);
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
        for (int x = 0; x < 1024; x++) {
            pBuf2[x + y * rect.Pitch] = 0;
        }
    }
    // 왼쪽부터 자르기
    if (m_eBarType == BAR_BULLET)
    {
        for (unsigned int y = 0; y < desc.Height; y++) {
            for (int x = 1024; x >= 1024 - 1024 * ratio; x--) {
                pBuf2[x + y * rect.Pitch] = pBuf[x + y * rect.Pitch];
            }
        }
    }
    // 오른쪽부터 자르기
    else
    {
        for (unsigned int y = 0; y < desc.Height; y++) {
            for (int x = 0; x < 512 * ratio; x++) {
                pBuf2[x + y * rect.Pitch] = pBuf[x + y * rect.Pitch];
            }
        }
    }
    pTexture->UnlockRect(0);
    pTexture2->UnlockRect(0);
    m_pGraphicDev->SetTexture(0, pTexture2);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    Render_Text();
}

void CBarUICamera::Render_Text()
{
    _tchar curValue[4];
    _tchar maxValue[4];
    switch (m_eBarType)
    {
    case BAR_HEALTH:
        _itow_s(m_pPlayer->Get_Health(), curValue, 10);
        _itow_s(m_pPlayer->Get_MaxHealth(), maxValue, 10);
        Engine::Render_Font(L"Font_Health", wstring(curValue), &_vec2(120.f, 510.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        Engine::Render_Font(L"Font_Health", L" / ", &_vec2(149.f, 510.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        Engine::Render_Font(L"Font_Health", wstring(maxValue), &_vec2(169.f, 510.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        break;
    case BAR_MANA:
        _itow_s(m_pPlayer->Get_Mana(), curValue, 10);
        _itow_s(m_pPlayer->Get_MaxMana(), maxValue, 10);
        Engine::Render_Font(L"Font_Health", wstring(curValue), &_vec2(95.f, 486.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        Engine::Render_Font(L"Font_Health", L" / ", &_vec2(115.f, 486.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        Engine::Render_Font(L"Font_Health", wstring(maxValue), &_vec2(135.f, 486.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        break;
    case BAR_BULLET:
        _itow_s(m_pPlayer->Get_Weapon()->Get_CurBullet(), curValue, 10);
        _itow_s(m_pPlayer->Get_Weapon()->Get_MaxBullet(), maxValue, 10);
        Engine::Render_Font(L"Font_Bullet", wstring(curValue), &_vec2(815.f, 495.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        Engine::Render_Font(L"Font_Bullet", L" / ", &_vec2(860.f, 495.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        Engine::Render_Font(L"Font_Bullet", wstring(maxValue), &_vec2(890.f, 495.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        break;
    }
}

HRESULT CBarUICamera::Add_Component()
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

CBarUICamera* CBarUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fWidth,
    const _float& fHeight, const _float& fPosX, const _float& fPosY,
    CPlayer*& pPlayer, const BARTYPE& eBarType, const _uint& uTexNum)
{
    CBarUICamera* pInstance = new CBarUICamera(pGraphicDev, pPlayer, eBarType, uTexNum);

    if (FAILED(pInstance->Ready_GameObject(fWidth, fHeight, fPosX, fPosY)))
    {
        Safe_Release(pInstance);
        MSG_BOX("BarUICamera Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CBarUICamera::Free()
{
    Engine::CUICamera::Free();
}