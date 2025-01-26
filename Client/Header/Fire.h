#pragma once
#include "InteractiveProp.h"

class CFire :
	public CInteractiveProp
{
#pragma once
public:
	explicit CFire(const CFire& rhs);

private:
	explicit CFire(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFire();

public:
	virtual HRESULT Ready_GameObject()											 override;
	virtual _int Update_GameObject(const _float& fTimeDelta)					 override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()											override;

	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	static CFire* Create(LPDIRECT3DDEVICE9	pGraphicDev);

public:
	bool Get_Dead() { return m_bDead; }
protected:
	virtual void Free() override;
	virtual HRESULT			Add_Component();
	virtual void TexNamer() override;

protected:
	void Detect_Player(const _float& fTimeDelta); //플레이어 거리 계산해서 Spike On/Off 판단, 추후 충돌처리로 대체

protected:
	bool m_bSpike;
	float m_fAtkTimer;
	CPlayer* m_pPlayer;
	//TODO : 도트 데미지 구현

private:
	bool		m_bDead;
	bool		m_bInit;
	_float		m_fKillTimer;	//삭제 시간 재는 용도 변수
	_float		m_fKillSecond;	//이 변수만큼 시간 이따가 삭제


};

