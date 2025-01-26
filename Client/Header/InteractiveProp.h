#pragma once
#include "Prop.h"
#include "Player.h"

class CInteractiveProp : public CProp
{
public:
	explicit CInteractiveProp(const CInteractiveProp& rhs);

protected:
	explicit CInteractiveProp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInteractiveProp();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;


public:
	static CInteractiveProp* Create(LPDIRECT3DDEVICE9	pGraphicDev);
protected:
	virtual void				Free() override;
	virtual  HRESULT			Add_Component() override;
	virtual void				TexNamer() override;

protected:
	float m_fHp;
	CPlayer* m_pPlayer;
	bool	 m_bInit;

};

