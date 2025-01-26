#pragma once
#include "Monster.h"
#include "MonsterBullet.h"

class CLooker : public CMonster
{
public:
	explicit CLooker(const CLooker& rhs);
private:
	explicit CLooker(LPDIRECT3DDEVICE9 pGraphicDev);
	
	virtual ~CLooker();

public:
	virtual HRESULT Ready_GameObject()						 override;
	HRESULT			Ready_GameObject(D3DXVECTOR3 vPos);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void	Render_GameObject()						 override;

	virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

private:
	HRESULT			Add_Component();

public:
	static CLooker* Create(LPDIRECT3DDEVICE9	pGraphicDev);
	static CLooker* Create(LPDIRECT3DDEVICE9	pGraphicDev, D3DXVECTOR3 vPos);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override; //���� ���ϸ� �ؽ�ó �ٲ��ִ� �Լ�
	void	Looker_Take_Dmg(const _float& fTimeDelta);

};

