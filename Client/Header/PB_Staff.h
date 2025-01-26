#pragma once

#include "Base.h"
#include "PlayerBullet.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CPB_Staff :	public CPlayerBullet
{
public:
	explicit CPB_Staff(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed);
	explicit CPB_Staff(const CPB_Staff& rhs);
	virtual ~CPB_Staff();

public:
	virtual void Render_GameObject()						 override;
	virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	static CPB_Staff*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg = 0, _float _fSpeed = 0.f);

};