#pragma once

#include "Camera.h"
#include "Engine_Define.h"
#include "Export_Utility.h"
#include "Weapon.h"

class CBulletCase;

class CPlayer : public Engine::CCamera
{

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAngle,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	HRESULT Ready_Clone_GameObject(CPlayer& rhs, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vAngle);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta) override;


public:
	const _vec3& Get_Player_Eye() { return m_vEye; }
	const _vec3& Get_Player_At() { return m_vAt; }
	const _vec3& Get_Player_Look() { return m_vInfo[INFO_POS] - m_vAt; }
	const _vec3& Get_Player_Up() { return m_vUp; }
	float Get_Player_Fov() { return m_fFov; }
	float Get_Player_Aspect() { return m_fAspect; }
	float Get_Player_Near() { return m_fNear; }
	float Get_Player_Far() { return m_fFar; }
	_bool Get_DoorGimmickOn() { return m_bDoorGimmickOn; }
	void Set_Player_Pos(const _vec3& vSrc) { m_pTransformCom->Set_Pos(vSrc); }
	void Set_Player_Speed(const _float& fRatio) { m_fCurSpeed *= fRatio; m_fSpeedTimer = 0.f; }   //매개변수로 현재 스피드에 비율 곱함
	void Set_Player_Reverse() { m_bReverse = true; m_fReverseTimer = 0.f;/*m_pGraphicDev->GetViewport(&originalVP)*/;}
	void Set_Player_ReverseOff() { m_bReverse = false; Engine::Get_SoundMgr()->InitFrequency(); }
	void Set_Player_Tilt() { m_fTilt = 0.f; }
	void Set_DoorGimmickOn() { m_bDoorGimmickOn = true; };
	_bool Get_Player_Reverse() { return m_bReverse; }

	void Set_FireLight();

public:
	CWeapon* Get_Weapon() { return m_mWeapon[m_eCurWeapon]; }
	CWeapon* Get_Weapon(WEAPON eWeapon) { return m_mWeapon[eWeapon]; }
	int Get_Health() { return m_iCurHealth; }
	int Get_MaxHealth() { return m_iMaxHealth; }
	int Get_Mana() { return m_iCurMana; }
	int Get_MaxMana() { return m_iMaxMana; }
	void		Add_Health(_int _iHealth) {
		if (_iHealth < 0)
		{
			Engine::Get_SoundMgr()->Play_Sound(L"Player_Hit.wav", EFFECT, 0.5f);
		}
		m_iCurHealth += _iHealth;
		m_iCurHealth = max(0, m_iCurHealth);
		m_iCurHealth = min(m_iMaxHealth, m_iCurHealth);
	}
	void		Add_Mana(_int _iMana) {
		m_iCurMana += _iMana;
		m_iCurMana = max(0, m_iCurMana);
		m_iCurMana = min(m_iMaxMana, m_iCurMana);
	}
	void		Add_Key(_int _iKeyNo) {
		m_bKey[_iKeyNo] = true;
	}

	WEAPON		Get_Cur_Weapon() { return m_eCurWeapon; }

	_int Get_Exp() { return m_iExp; }
	void Add_Exp(int _iExp) { m_iExp += _iExp; m_iExp = max(0, m_iExp); }
	_bool Get_Nearby() { return m_bNearby; }
	void Set_Nearby(_bool _bNearby) { m_bNearby = _bNearby; }

	_bool Get_Key(DOORID eColor) { return m_bKey[eColor]; }
	void  Set_Key(DOORID eColor) { m_bKey[eColor] = true; }
	_bool Get_ShopOn() { return m_bShopOn; }
	void Set_ShopOn(_bool _bShopOn) { m_bShopOn = _bShopOn; }

	bool Is_Collider_Visible() { return m_ColliderToggle; }
	bool Is_Moving() { return m_bMoving; }

	void Set_Camera_Shaking() { m_bShaking = true; }

	void Calculate_Degree(_vec3 vPos);

public:
	virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta);
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Change_Weapon();
	void		Mouse_Move();
	void		UpdateCamera(const _float& fTimeDelta);
	void		Shake_Camera(const _float& fTImeDelta);
	HRESULT		Add_Component(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAngle);

private:
	Engine::CTransform* m_pTransformCom;

	_int			m_iMaxHealth;
	_int			m_iCurHealth;
	_int			m_iPreHealth;
	_int			m_iMaxMana;
	_int			m_iCurMana;
	_int			m_iExp;
	WEAPON			m_eCurWeapon;
	map<WEAPON,		CWeapon*> m_mWeapon;
	_float			m_fSpeed;
	D3DXVECTOR3		m_vInfo[INFO_END];
	D3DXVECTOR3		m_vAngle;

	_float      m_fCurSpeed;      //현재 플레이어 속도
	_float      m_fSpeedTimer;
	_bool      m_bKey[DOOR_END];
	_float		m_fHitDelay;

	//혼란
	_bool      m_bReverse;
	_float      m_fReverseTimer;
	bool	  m_ColliderToggle = true;
	bool	  Key_L_Pressed = false;
	bool	  m_bMoving = false;
	bool	  m_bIsDamaged = false;
	bool	  m_bShaking = false;
	_bool	  m_bShopOn = false;
	_bool	  m_bNearby = false;
	_float	  m_fElapsedShakingTime = 0.f;
	_float	  m_fDieAwayShakingTime = 0.f;
	float	  m_fShakeYAngle = 0.f;
	float     m_shakeAmount;

	_float		m_fTilt;
	_bool bIncrease = false; // 새로 추가된 변수, 처음에는 감소하는 방향으로 설정합니다.
	_bool m_bDoorGimmickOn = false;


public:
	_vec3	  m_vDir;

public:
	static CPlayer* Clone_Create(CPlayer& rhs, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vAngle);

	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAngle,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

private:
	virtual void Free();
};