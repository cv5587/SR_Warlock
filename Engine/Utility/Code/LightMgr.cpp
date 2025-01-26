#include "..\..\Header\LightMgr.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
    for (int i = 0; i < sizeof(m_bLight) / sizeof(bool); i++)
    {
        m_bLight[i] = false;
    }
}

CLightMgr::~CLightMgr()
{
    Free();
}

HRESULT CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
    const D3DLIGHT9* pLightInfo,
    const _uint& iIndex)
{
    CLight* pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
    NULL_CHECK_RETURN(pLight, E_FAIL);

    m_arrLight[iIndex] = pLight;

    return S_OK;
}

int CLightMgr::Enable_Light(LPDIRECT3DDEVICE9 pGraphicDev, const LIGHTID& eLightID,
    const _uint& iChannelID, const _bool& bEnable, const _vec3& vPos, const _float& fRange)
{
    // ±íÀº º¹»ç
    D3DLIGHT9 light;
    memcpy(&light, &m_arrLight[eLightID]->Get_LightInfo(), sizeof(D3DLIGHT9));
    light.Position = vPos;
    if (fRange > 0.f) light.Range = fRange;

    int channelID(iChannelID);
    // ºó Ã¤³Î Å½»ö
    if (channelID == -1 || channelID >= sizeof(m_bLight) / sizeof(_bool))
    {
        if (!bEnable)
            return -1;
        for (int i = 0; i < sizeof(m_bLight) / sizeof(_bool); i++)
        {
            if (!m_bLight[i])
            {
                channelID = i;
                break;
            }
        }
    }

    pGraphicDev->SetLight(channelID, &light);
    if (bEnable)
    {
        ///if (!m_bLight[channelID])
        {
            pGraphicDev->LightEnable(channelID, TRUE);
            m_bLight[channelID] = true;
        }
        return channelID;
    }
    else if (m_bLight[channelID])
    {
        pGraphicDev->LightEnable(channelID, FALSE);
        m_bLight[channelID] = false;
        return -1;
    }
}

void CLightMgr::Clear_Light(LPDIRECT3DDEVICE9 pGraphicDev)
{
    for (int i = 1; i < sizeof(m_bLight) / sizeof(_bool); i++)
    {
        pGraphicDev->LightEnable(i, FALSE);
        m_bLight[i] = false;
    }
}

void CLightMgr::Free()
{
    for (auto iter : m_arrLight)
    {
        Safe_Release(iter);
    }
}