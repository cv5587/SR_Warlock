#pragma once
#include "Monster.h"
#include "IceBullet.h"
#include "Boss_Tentacle.h"

class CBoss_Phase2 : public CMonster
{
public:
	explicit CBoss_Phase2(const CBoss_Phase2& rhs);

private:
	explicit CBoss_Phase2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss_Phase2();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	override;
	virtual void Render_GameObject()						 override;


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	void			Set_Active() { m_bActive = true; m_bInit = true; Engine::Get_SoundMgr()->StopSound(BGM); }

private:
	HRESULT			Add_Component();
	void Elevate(const _float& fTimeDelta);
	bool m_bIsSoundStarted = false;

public:
	static CBoss_Phase2* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override; //상태 변하면 텍스처 바꿔주는 함수
	void			Boss_Phase2_Take_Dmg(const _float& fTimeDelta);
	void			Boss_SlowBullet(_vec3 vSrc, _vec3 vDst, _float fAngle);
private:
	enum BOSS2STATE {
		BOSS2_INACTIVE,
		BOSS2_IDLE,
		BOSS2_ATK,
		BOSS2_WARP,
		BOSS2_MAGIC,
		BOSS2_SUMMON,
		BOSS2_ICE,
		BOSS2_HIT,
		BOSS2_DEAD,
		BOSS2_END
	};

	BOSS2STATE		m_eBoss2State;
	BOSS2STATE		m_ePreBoss2State;

	list<CIceBullet*>		m_lSpikes;
	list<CBoss_Tentacle*>		m_lTentacles;

	_int				m_iBulletCount;
	_int				m_iAlpha = 255;
	_bool				m_bWarp;
	_bool				m_bSuperArmor;	//true 시 무적상태
	_bool				m_bActive;
	_bool				m_bElevate;
	_int				m_iWarpPhase;
	_int				m_iNoRandom;

	_float				m_fPatTimer; //패턴 타이머
	_int				m_iSummonCnt; //소환 카운터, 5되면 소환
	//_int				m_iPreHealth;
	//추가 텍스처 컴포넌트
};

