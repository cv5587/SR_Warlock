#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Light.h"


BEGIN(Engine)


class ENGINE_DLL CLightMgr : public CBase
{
    DECLARE_SINGLETON(CLightMgr)

private:
    explicit CLightMgr();
    virtual ~CLightMgr();

public:
    HRESULT      Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
        const D3DLIGHT9* pLightInfo,
        const _uint& iIndex);
    HRESULT      Ready_Material(D3DMATERIAL9* pMaterialInfo, const MATERIALID& iIndex = MTRL_DEFAULT) { m_arrMaterial[iIndex] = pMaterialInfo; return S_OK; }
    int Enable_Light(LPDIRECT3DDEVICE9 pGraphicDev, const LIGHTID& eLightID = LIGHT_DEFAULT, const _uint& iChannelID = 0,
        const _bool& bEnable = true, const _vec3& vPos = { 0.f, 0.f, 0.f }, const _float& fRange = -1.f);
    void Enable_Material(LPDIRECT3DDEVICE9 pGraphicDev, const MATERIALID& iIndex = MTRL_DEFAULT) {
        pGraphicDev->SetMaterial(m_arrMaterial[iIndex]);
    };
    const D3DLIGHT9& Get_Light(const LIGHTID& eLightID) {
        return m_arrLight[eLightID]->Get_LightInfo();
    }
    void Set_Light(const LIGHTID& eLightID, const D3DLIGHT9 eLight) {
        m_arrLight[eLightID]->Set_LightInfo(eLight);
    }
    void Clear_Light(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    _bool      m_bLight[100];
    CLight* m_arrLight[LIGHT_END];
    D3DMATERIAL9* m_arrMaterial[MTRL_END];

private:
    virtual void   Free();

};

END