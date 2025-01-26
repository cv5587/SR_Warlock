#pragma once

#include "UICamera.h"
#include "Engine_Define.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CScreenEffect : public Engine::CUICamera
{
private:
    explicit CScreenEffect(LPDIRECT3DDEVICE9 pGraphicDev, _int _iTexNum);
    explicit CScreenEffect(const CScreenEffect& rhs);
    virtual ~CScreenEffect();

public:
    HRESULT Ready_GameObject();

    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject(const float& fTimeDelta) override;
    virtual void Render_GameObject();

private:
    HRESULT         Add_Component();

private:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CTexture* m_pTextureCom;
    _float            m_fFrame = 0.f;
    int               m_iMaxFrame;
    _int                m_iTexNum;

public:
    static CScreenEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _TexNum);

private:
    virtual void Free();
};