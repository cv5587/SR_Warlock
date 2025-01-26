#pragma once
#include "Monster.h"
#include <deque>

class CYeti : public CMonster
{
private:
	explicit CYeti(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CYeti(const CYeti& rhs);
	virtual ~CYeti();

public:
	virtual HRESULT Ready_GameObject()                   override;
	HRESULT			Ready_GameObject(D3DXVECTOR3 vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)                override;
	virtual void Render_GameObject()                   override;

	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;


private:
	HRESULT         Add_Component();

public:
	static CYeti* Create(LPDIRECT3DDEVICE9   pGraphicDev);
	static CYeti* Create(LPDIRECT3DDEVICE9   pGraphicDev, D3DXVECTOR3 vPos);

private:
	virtual void Free() override;

	virtual void   Motion_Change()                      override;
	void   Yeti_Take_Dmg(const _float& fTimeDelta);
private:

	//Textrue Components
	Engine::CTexture* m_pTexCom_Idle;
	Engine::CTexture* m_pTexCom_Run;
	Engine::CTexture* m_pTexCom_Atk;
	Engine::CTexture* m_pTexCom_Hit;
	Engine::CTexture* m_pTexCom_Dead;

	_float      m_fRunTimer;         //뛰기 시작한 시간 타이머
	_float      m_fInitSpeed;         //시작 속도
	const _float      m_fMaxSpeed;   //최고속도
	_float      m_fAccel;            //가속도
	_bool		m_bAtk2 = false;

	deque<_vec3*> m_tFramePos;
};
