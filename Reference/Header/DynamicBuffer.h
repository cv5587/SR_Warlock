#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicBuffer :
    public CComponent
{
protected:
	explicit CDynamicBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicBuffer(const CDynamicBuffer& rhs);
	virtual ~CDynamicBuffer();

public:
	virtual HRESULT			Ready_Buffer();
	virtual _int			Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void			Render_Buffer();

public:
	virtual void Free();

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

	_ulong		m_dwVtxCnt;
	_ulong		m_dwVtxSize;
	_ulong		m_dwTriCnt;
	_ulong		m_dwFVF;

	_ulong		m_dwIdxSize;
	D3DFORMAT	m_IdxFmt;

};

END