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

class CBarUICamera : public Engine::CUICamera
{
private:
    explicit CBarUICamera(LPDIRECT3DDEVICE9 pGraphicDev, CPlayer* pPlayer, BARTYPE eBarType, _int uTexNum);
    explicit CBarUICamera(const CBarUICamera& rhs);
    virtual ~CBarUICamera();

public:
    HRESULT Ready_GameObject(
        const _float& fWidth,
        const _float& fHeight,
        const _float& fPosX,
        const _float& fPosY);

    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject(const float& fTimeDelta) override;
    virtual void Render_GameObject();
    void Render_Text();

private:
    HRESULT         Add_Component();

private:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CTexture* m_pTextureCom;
    _float   m_fFrame = 0.f;
    _int m_iTexNum;
    CPlayer* m_pPlayer;
    BARTYPE m_eBarType;

public:
    static CBarUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
        const _float& fWidth,
        const _float& fHeight,
        const _float& fPosX,
        const _float& fPosY,
        CPlayer*& pPlayer,
        const BARTYPE& eBarType,
        const _uint& uTexNum = 0
    );

private:
    virtual void Free();
};