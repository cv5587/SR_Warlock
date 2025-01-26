#pragma once
#include "Weapon.h"
class CClaymore : public CWeapon
{
private:
	explicit CClaymore(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CClaymore(const CClaymore& rhs);
	virtual ~CClaymore();

public:
	virtual HRESULT		Ready_GameObject();
	HRESULT				Ready_Clone_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject(const float& fTimeDelta);
	virtual void		Render_GameObject();
	virtual void		Attack(_vec3 vPos, _vec3 vDir);

private:
	HRESULT			Add_Component();

public:
	static CClaymore* Create(LPDIRECT3DDEVICE9	pGraphicDev);
	static CClaymore* Clone_Create(const CClaymore& rhs);

private:
	virtual void Free() override;
};


