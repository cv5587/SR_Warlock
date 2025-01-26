#include "..\..\Header\Light.h"
#include "LightMgr.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_iIndex(0)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
	m_pGraphicDev->AddRef();
}

CLight::~CLight()
{
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 * pLightInfo,
							const _uint & iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	m_pGraphicDev->SetLight(iIndex, pLightInfo);

	return S_OK;
}

HRESULT CLight::Ready_Light(LIGHTID eLightID)
{
	const D3DLIGHT9& LightInfo = CLightMgr::GetInstance()->Get_Light(eLightID);

	memcpy(&m_tLightInfo, &LightInfo, sizeof(D3DLIGHT9));
	m_eLightID = eLightID;

	return S_OK;
}
CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev,
						const D3DLIGHT9* pLightInfo,
						const _uint& iIndex)
{
	CLight *	pInstance = DBG_NEW CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
	{
		MSG_BOX("CLight Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, LIGHTID m_eLightID)
{
	CLight* pInstance = DBG_NEW CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(m_eLightID)))
	{
		MSG_BOX("CLight Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pGraphicDev);
}
