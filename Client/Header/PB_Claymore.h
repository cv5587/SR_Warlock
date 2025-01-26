#pragma once

#include "Base.h"
#include "PlayerBullet.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CPB_Claymore : public CPlayerBullet
{
public:
	explicit CPB_Claymore(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg);
	explicit CPB_Claymore(const CPB_Claymore& rhs);
	virtual ~CPB_Claymore();

public:
	virtual HRESULT Ready_GameObject()   override;
	virtual void Render_GameObject()                   override;
	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;

	virtual void   Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	static CPB_Claymore* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iDmg = 0);

};