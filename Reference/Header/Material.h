#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMaterial : public CBase
{
private:
	explicit CMaterial(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMaterial();

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	D3DMATERIAL9				m_tMaterialInfo;

public:
	HRESULT		Ready_Material(const D3DMATERIAL9* pMaterialInfo);

public:
	static CMaterial*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const D3DMATERIAL9* pMaterialInfo);
private:
	virtual void		Free();

};

END