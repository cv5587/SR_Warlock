#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Player.h"
#include "MonsterBullet.h"
#include "Effect.h"
#include "Firework.h"
#include "MonsterHpUI.h"
#include "PlayerBullet.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CMonster : public Engine::CGameObject
{
public:
	explicit CMonster(const CMonster& rhs);


protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;

	virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	void						Take_Dmg(int iDmg) { if (this != nullptr) { m_iCurHealth -= iDmg; m_bHit = true; } }; //데미지 입힘
	int							Get_Dmg() { return m_iDamage; }
	float						Get_HpRatio() { return (float)m_iCurHealth / (float)m_iMaxHealth; }

	void						Set_Detect_Range(_float detectRange) { m_fDtctRng = detectRange; }
protected:
	virtual  HRESULT			Add_Component();
	virtual void				Motion_Change();	//상태 변하면 텍스처 바꿔주는 함수

	_float						DistanceToPlayer(CPlayer* pPlayer); // 플레이어 위치 == 카메라 Eye거리 계산

	//밑 두 함수는 업뎃에서 사용
	void						Chase_Player(CPlayer* pPlayer, const _float& fTimeDelta); //플레이어 추적
	void						Knock_Back(CPlayer* pPlayer, //넉백 : 플레이어 반대 방향으로 밀려남
										const _float& fSpeed, //밀려나는 거리
										const _float& fTimeDelta);



protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom = nullptr;

protected:
	enum MONSTERSTATE	//일반 몬스터 상태
	{
		MONSTER_IDLE,
		MONSTER_RUN,
		MONSTER_ATK,
		MONSTER_HIT,
		MONSTER_DEAD,
		MONSTER_END
	};

	MONSTERSTATE	m_eState;
	MONSTERSTATE	m_ePreState;

	//Textrue Components
	Engine::CTexture* m_pTexCom_Idle;
	Engine::CTexture* m_pTexCom_Run;
	Engine::CTexture* m_pTexCom_Atk;
	Engine::CTexture* m_pTexCom_Hit;
	Engine::CTexture* m_pTexCom_Dead;

	CPlayer*			m_pPlayer;
	CMonsterHpUI*		m_pMonsterHpUI;

	_float				m_fFrame = 0.f;
	_uint				m_iMaxFrame = 1;
	_float				m_fFrameSpeed = 0.f;
	wstring				m_szTexName;	//텍스처 이름
	wstring				m_szName;	//텍스처 이름

	_float		m_fAtkTimer;	//공격 시간 재는 변수
	_float		m_fStiffTimer;	//경직 시간 재는 변수
	_float		m_fStiffSecond; //경직 시간

	//몬스터 기본 스탯
	_int		m_iMaxHealth;
	_int		m_iCurHealth;
	_int		m_iDamage;
	_float		m_fSpeed;
	_float		m_fDtctRng;
	_float		m_fAtkRng;

	bool		m_bDead;
	bool		m_bInit;
	bool		m_bHit;
	_bool		m_bAtk;

public:
	static CMonster*		Create(LPDIRECT3DDEVICE9	pGraphicDev);

protected:
	virtual void Free();
};

