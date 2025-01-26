#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator : public CComponent
{
private:
	explicit CAnimator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimator(const CAnimator& rhs);
	virtual ~CAnimator();

public:
	HRESULT			Ready_Animator(int iMaxFrame);
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual void	LateUpdate_Component();

public:
	//Render �� Set_Texture �� Get_Frame() �־��ֱ�
	_float			Get_Frame() { return m_fFrame; }

public:
	virtual CComponent* Clone();
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphicDev, int iMaxFrame);


private:
	virtual void		Free();

private:
	_float			m_fFrame;
	_int			m_iMaxFrame;

};

END
