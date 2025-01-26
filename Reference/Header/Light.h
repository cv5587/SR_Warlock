#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLight :	public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight();

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_uint						m_iIndex;

public:
	
	const D3DLIGHT9& Get_LightInfo()		{ return m_tLightInfo; }
	void			 Set_LightInfo(const D3DLIGHT9& lightInfo)
	{
		memcpy(&m_tLightInfo, &lightInfo, sizeof(D3DLIGHT9));
	}
	const LIGHTID& Get_LightID()				{ return m_eLightID; }
	void Set_LightID(const LIGHTID& eLightID)	{ m_eLightID = eLightID; }

	D3DXVECTOR3		 Get_Light_Pos() { return m_tLightInfo.Position; }
	void			 Set_Light_Pos(D3DXVECTOR3 pos) { m_tLightInfo.Position = pos; }

	float			 Get_Light_Range() { return m_tLightInfo.Range; }
	void			 Set_Light_Range(float range) { m_tLightInfo.Range = range; }



public:
	HRESULT		Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);

	HRESULT		Ready_Light(LIGHTID eLightID);

public:
	static CLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const D3DLIGHT9* pLightInfo, 
								const _uint& iIndex);

	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev, LIGHTID m_eLightID);

private:
	virtual void		Free();

private:
	D3DLIGHT9					m_tLightInfo;
	LIGHTID						m_eLightID = LIGHT_DEFAULT;
};

END