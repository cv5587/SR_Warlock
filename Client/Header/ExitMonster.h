#pragma once
#include "Monster.h"
#include "MonsterBullet.h"

class CExitMonster : public CMonster
{
private:
	explicit CExitMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CExitMonster(const CExitMonster& rhs);
	virtual ~CExitMonster();

public:
	virtual HRESULT Ready_GameObject()						 override;
	HRESULT			Ready_GameObject(D3DXVECTOR3 vPos);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void	Render_GameObject()						 override;

	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

private:
	HRESULT			Add_Component();

public:
	static CExitMonster* Create(LPDIRECT3DDEVICE9	pGraphicDev);
	static CExitMonster* Create(LPDIRECT3DDEVICE9	pGraphicDev, D3DXVECTOR3 vPos);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override;
	void	ExitMonster_Take_Dmg(const _float& fTimeDelta);

private:

};

//TODO :
//1. ���� 2������ �ؽ�ó �ֱ�
//2. ���� ��ŸŬ ����� �� ���۾Ƹ�
//3. ���� ������ Ű���