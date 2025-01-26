#pragma once
#include "UICamera.h"
#include "Engine_Define.h"
#include "Export_Utility.h"
#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CShopUICamera : public Engine::CUICamera
{
private:
    explicit CShopUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer);
    explicit CShopUICamera(const CShopUICamera& rhs);
    virtual ~CShopUICamera();

public:
    HRESULT Ready_GameObject(
        const _float& fWidth,
        const _float& fHeight,
        const _float& fPosX,
        const _float& fPosY);

    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject(const float& fTimeDelta) override;
    virtual void Render_GameObject();
    int Render_Upgrade();
    int Render_ArrowL();
    int Render_ArrowR();
    int Render_Weapon(WEAPON eWeapon, _float fScaleX, _float fScaleY, _float fPosX, _float fPosY);
    int Render_Exit();
    _bool IsActive() { return m_bShowShop; }
    _bool   Get_ShowShop() { return m_bShowShop; }
    void   Set_ShowShop() { m_bShowShop = !m_bShowShop; }
private:
    HRESULT         Add_Component();

private:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom[20];
    Engine::CTexture* m_pTextureCom[2];
    CPlayer* m_pPlayer;
    _bool m_bShowShop;
    POINT m_ptMouse;
    _bool m_bAlreadyGet[WEAPON_END];
    WEAPON m_eSelectedWeapon;
    _float m_fFrame;

public:
    static CShopUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
        const _float& fWidth,
        const _float& fHeight,
        const _float& fPosX,
        const _float& fPosY,
        CPlayer*& pPlayer
    );

private:
    virtual void Free();
};