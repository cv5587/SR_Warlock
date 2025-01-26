#pragma once
#include "Weapon.h"
class CReactor : public CWeapon
{
private:
	explicit CReactor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CReactor(const CReactor& rhs);
	virtual ~CReactor();

public:
	virtual HRESULT		Ready_GameObject();
	HRESULT				Ready_Clone_GameObject();

	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject(const float& fTimeDelta);
	virtual void		Render_GameObject();
	_bool				Get_Charge() { return m_bCharge; }
	virtual void		Attack(_vec3 vPos, _vec3 vDir);
	virtual bool		Get_Attack() { return __super::Get_Attack() || m_bCharge || m_bShoot; }

private:
	HRESULT			Add_Component();
	_bool		m_bCharge;
	_bool		m_bShoot;

public:
	static CReactor* Create(LPDIRECT3DDEVICE9	pGraphicDev);
	static CReactor* Clone_Create(const CReactor& rhs);

private:
	virtual void Free() override;
};


