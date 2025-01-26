#pragma once
#include "Monster.h"

class CBoss_Phase1 : public CMonster
{
public:
	explicit CBoss_Phase1(const CBoss_Phase1& rhs);

private:
	explicit CBoss_Phase1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss_Phase1();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	override;
	virtual void Render_GameObject()						 override;


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

private:
	HRESULT			Add_Component();

public:
	static CBoss_Phase1* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override; //상태 변하면 텍스처 바꿔주는 함수
	void	Boss_Phase1_Take_Dmg(const _float& fTimeDelta);
	void	Boss_SlowBullet(_vec3 vSrc, _vec3 vDst, _float fAngle);
private:
	enum BOSS1STATE {
		BOSS1_IDLE,
		BOSS1_RUN,
		BOSS1_ATK,
		BOSS1_WARP,
		BOSS1_MAGIC,
		BOSS1_HALLUCINATE,
		BOSS1_HIT,
		BOSS1_DEAD,
		BOSS1_END
	};

	BOSS1STATE		m_eBoss1State;
	BOSS1STATE		m_ePreBoss1State;

	_int				m_iBulletCount;
	_int				m_iAlpha = 255;
	bool				m_bWarp;
	bool				m_bSuperArmor;	//true 시 무적상태
	_int				m_iWarpPhase;
	_int				m_iNoRandom;
	_float				m_fPatTimer;
	_float				m_fPatSecond = 5.f;

	_int				m_iPreHealth;
	_bool				m_bHallucinate;
	_int				m_iHalluPhase;
	_bool				m_bHalluDmged;
	array<_vec3, 5>		m_tHalluPoses;
	_int				m_iRealIndex;
	//추가 텍스처 컴포넌트
};

