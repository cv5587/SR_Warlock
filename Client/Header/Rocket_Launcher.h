#pragma once
#include "Weapon.h"
#include "Smoke.h"
class CRocket_Launcher : public CWeapon
{
public:
	explicit CRocket_Launcher(const CRocket_Launcher& rhs);

private:
	explicit CRocket_Launcher(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRocket_Launcher();

public:
	virtual HRESULT		Ready_GameObject();
	HRESULT				Ready_Clone_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject(const float& fTimeDelta);
	virtual void		Render_GameObject();
	virtual void		Attack(_vec3 vPos, _vec3 vDir);
	virtual void		Upgrade_Weapon();

private:
	HRESULT			Add_Component();

private:
	list<CSmoke*> m_lSmoke;

public:
	static CRocket_Launcher* Create(LPDIRECT3DDEVICE9	pGraphicDev);
	static CRocket_Launcher* Clone_Create(const CRocket_Launcher& rhs);
private:
	virtual void Free() override;
};


