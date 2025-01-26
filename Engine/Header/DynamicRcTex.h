#pragma once
#include "DynamicBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicRcTex :
    public CDynamicBuffer
{
private:
	explicit CDynamicRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicRcTex(const CDynamicRcTex& rhs);
	virtual ~CDynamicRcTex();

public:
	virtual HRESULT		Ready_Buffer(bool DirisRight, float width, float height);
	virtual _int		Update_Component(const _float& fTimeDelta);
	virtual void		Render_Buffer();

public:
	static CDynamicRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, bool DirisRight, float width = 2.f, float height = 2.f);
	virtual CComponent* Clone();

private:
	float m_Us[2];
	bool m_bDirisRight = false;
private:
	virtual void	Free();
};

END