#pragma once

#include "Base.h"
#include "PlayerBullet.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CPB_None : public CPlayerBullet
{
public:
	explicit CPB_None(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg, _float _fSpeed);
	explicit CPB_None(const CPB_None& rhs);
	virtual ~CPB_None();

public:
	virtual void Render_GameObject()                   override;
	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;

	virtual void   Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	static CPB_None* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg = 0, _float _fSpeed = 0.f);

};