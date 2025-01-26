#pragma once
#include "Prop.h"
#include	"Player.h"
class CBreakableProp : public CProp
{
public:
	explicit CBreakableProp(const CBreakableProp& rhs);

protected:
	explicit CBreakableProp(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID);
	virtual ~CBreakableProp();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	static CBreakableProp* Create(LPDIRECT3DDEVICE9	pGraphicDev);
protected:
	virtual void				Free() override;
	virtual  HRESULT			Add_Component() override;
	virtual void				TexNamer() override;

protected:
	_int	m_iHp;
	_int	m_iMaxHp;
	bool	m_bDead;
	BOXID	m_eBoxID;

};

