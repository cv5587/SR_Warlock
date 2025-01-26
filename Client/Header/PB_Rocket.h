#pragma once

#include "Base.h"
#include "PlayerBullet.h"
#include <Smoke.h>

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CPB_Rocket :	public CPlayerBullet
{
public:
	explicit CPB_Rocket(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed);
	explicit CPB_Rocket(const CPB_Rocket& rhs);
	virtual ~CPB_Rocket();

public:
	virtual HRESULT Ready_GameObject()   override;
	virtual int Update_GameObject(const float& fTimeDelta);
	virtual void Render_GameObject()						 override;
	virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta);
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	static CPB_Rocket*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg = 0, _float _fSpeed = 0.f);

protected:
	virtual void Free() override;
};