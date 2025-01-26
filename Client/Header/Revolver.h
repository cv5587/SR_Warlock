#pragma once
#include "Weapon.h"
class CRevolver : public CWeapon
{
private:
	explicit CRevolver(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRevolver(const CRevolver& rhs);
	virtual ~CRevolver();

public:
	virtual HRESULT		Ready_GameObject();
	HRESULT				Ready_Clone_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject(const float& fTimeDelta);
	virtual void		Render_GameObject();
	virtual void		Upgrade_Weapon();
	virtual void		Attack(_vec3 vPos, _vec3 vDir);

private:
	HRESULT			Add_Component();

public:
	static CRevolver* Create(LPDIRECT3DDEVICE9	pGraphicDev);
	static CRevolver* Clone_Create(const CRevolver& rhs);


private:
	virtual void Free() override;
};


