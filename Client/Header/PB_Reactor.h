#pragma once

#include "Base.h"
#include "PlayerBullet.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CPB_Reactor :	public CPlayerBullet
{
public:
	explicit CPB_Reactor(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed);
	explicit CPB_Reactor(const CPB_Reactor& rhs);
	virtual ~CPB_Reactor();

public:
	virtual HRESULT Ready_GameObject()   override;
	virtual int Update_GameObject(const float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;
	virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;
	void Set_Divided() { m_bDivided = true; }

private:
	_bool	m_bDivided;
	_bool m_bAttack;
	_float m_fFrame;

public:
	static CPB_Reactor*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg = 0, _float _fSpeed = 0.f);

};