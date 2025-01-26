#pragma once
#include "Weapon.h"
class CShotgun : public CWeapon
{
private:
	explicit CShotgun(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShotgun(const CShotgun& rhs);
	virtual ~CShotgun();

public:
	virtual HRESULT		Ready_GameObject();
	HRESULT				Ready_Clone_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject(const float& fTimeDelta);
	virtual void		Render_GameObject();
	virtual void		Attack(_vec3 vPos, _vec3 vDir);
	virtual void		Upgrade_Weapon();
	virtual bool		Get_Attack() { return __super::Get_Attack() || m_bReload; }
private:
	HRESULT			Add_Component();
	_bool		m_bReload;
	_int		m_bBulletCount;

public:
	static CShotgun* Create(LPDIRECT3DDEVICE9	pGraphicDev);
	static CShotgun* Clone_Create(const CShotgun& rhs);
private:
	virtual void Free() override;
};


