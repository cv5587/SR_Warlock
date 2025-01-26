#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();

public:
	virtual HRESULT		Ready_Buffer(float width, float height);
	virtual void		Render_Buffer();

public:
	static CRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, float width, float height);
	virtual CComponent*	Clone();

private:
	virtual void	Free();
};

END