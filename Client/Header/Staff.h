#pragma once
#include "Weapon.h"
class CStaff : public CWeapon
{
private:
	explicit CStaff(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaff(const CStaff& rhs);
	virtual ~CStaff();

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
	static CStaff* Create(LPDIRECT3DDEVICE9	pGraphicDev);
	static CStaff* Clone_Create(const CStaff& rhs);

private:
	virtual void Free() override;
};


