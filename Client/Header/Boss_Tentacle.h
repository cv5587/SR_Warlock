#pragma once
#include "Monster.h"
#include "MonsterBullet.h"

class CBoss_Tentacle : public CMonster
{
public:
	explicit CBoss_Tentacle(const CBoss_Tentacle& rhs);

private:
	explicit CBoss_Tentacle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss_Tentacle();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;

	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

	_bool			Get_Dead() { return m_bDead; }
private:
	HRESULT			Add_Component();

public:
	static CBoss_Tentacle* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override; //상태 변하면 텍스처 바꿔주는 함수
	void	Boss_Tentacle_Take_Dmg(const _float& fTimeDelta);
private:
	_bool				 m_bSuperArmor;
	_bool				 m_bDead;
	_int				 m_iPreHealth;
};

//TODO :
//1. 보스 2페이즈 텍스처 넣기
//2. 보스 텐타클 사라질 때 슈퍼아머
//3. 보스 사이즈 키우기