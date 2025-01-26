#pragma once

#include "Base.h"
#include "GameObject.h"
#include "BulletCase.h"
#include "PlayerBullet.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "Effect.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CWeapon : public Engine::CGameObject
{
protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon();

public:
	int Get_CurBullet() { return m_iCurBullet; }
	int Get_MaxBullet() { return m_iMaxBullet; }
	void Add_Bullet(_int _iBullet) {
		m_iCurBullet += _iBullet;
		m_iCurBullet = max(0, m_iCurBullet);
		m_iCurBullet = min(m_iMaxBullet, m_iCurBullet);
	}
	int Get_Damage() { return m_iDamage; }
	virtual _int      Update_GameObject(const float& fTimeDelta);
	virtual void      LateUpdate_GameObject(const float& fTimeDelta);
	virtual void Attack(_vec3 vPos, _vec3 vDir) PURE;
	virtual void Upgrade_Weapon() {};
	virtual _bool Get_Upgrade() { return m_bUpgraded; }
	_int Get_Upgrade_Cost() { return m_iUpgrade_cost; }
	virtual void Show_Effect(_vec3 vPos) {};
	virtual bool Get_Attack();

public:
	_int m_iMaxBullet;
	_int m_iCurBullet;
	_int m_iDamage;
	_bool m_bAttack;
	_float m_fFrame;
	_float m_fLightFrame;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pUITextureCom;
	_vec3 m_vBulletCaseDir;
	_bool m_bUpgraded;
	_int m_iUpgrade_cost;


protected:
	virtual void Free() override;
};