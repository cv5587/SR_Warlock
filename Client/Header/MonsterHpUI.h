#pragma once

#include "UICamera.h"
#include "Engine_Define.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CMonsterHpUI : public Engine::CUICamera
{
private:
    explicit CMonsterHpUI(LPDIRECT3DDEVICE9 pGraphicDev, wstring szName);
    explicit CMonsterHpUI(const CMonsterHpUI& rhs);
    virtual ~CMonsterHpUI();

public:
    HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& fTimeDelta) override;
    virtual void LateUpdate_GameObject(const float& fTimeDelta) override;
    virtual void Render_GameObject();
    void Render_Text();
    virtual void Set_Ratio(float _fRatio) { m_fRatio = _fRatio; m_fFrame = 0.f; }

private:
    HRESULT         Add_Component();

private:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom[2];
    Engine::CTexture* m_pTextureCom;
    _bool m_bActive = false;
    _float   m_fFrame = 0.f;
    _float m_fRatio = 0.f;
    wstring m_szName = L"";

public:
    static CMonsterHpUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring szName);

private:
    virtual void Free();
};