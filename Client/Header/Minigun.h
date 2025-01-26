#pragma once
#include "Weapon.h"

class CMinigun : public CWeapon
{
private:
	explicit CMinigun(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMinigun(const CMinigun& rhs);
	virtual ~CMinigun();

public:
	virtual HRESULT   Ready_GameObject();
	HRESULT			  Ready_Clone_GameObject();
	virtual _int      Update_GameObject(const _float& fTimeDelta);
	virtual void      LateUpdate_GameObject(const float& fTimeDelta);
	virtual void      Render_GameObject();
	virtual void      Attack(_vec3 vPos, _vec3 vDir);
	virtual void      Upgrade_Weapon();

private:
	HRESULT         Add_Component();

private:
	_int m_iAttackDelay;

public:
	static CMinigun* Create(LPDIRECT3DDEVICE9   pGraphicDev);
	static CMinigun* Clone_Create(const CMinigun& rhs);

private:
	virtual void Free() override;
};