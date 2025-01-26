#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUICamera2 : public CGameObject
{
protected:
	explicit CUICamera2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUICamera2();

public:
	virtual HRESULT Ready_GameObject();

protected:
	_matrix		m_matView, m_matProj;

public:
	static CUICamera2* Create(LPDIRECT3DDEVICE9	pGraphicDev);

protected:
	virtual void	Free();
};

END