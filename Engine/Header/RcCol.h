#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
private:
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol();

public:
	virtual HRESULT		Ready_Buffer(float width, float height, const D3DXCOLOR& color);
	virtual void		Render_Buffer();

public:
	static CRcCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float width, float height, const D3DXCOLOR& color);
	static CRcCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone();

private:
	virtual void	Free();
};

END