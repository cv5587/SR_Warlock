#pragma once
#include "Monster.h"
#include "IceBullet.h"

class CShambler : public CMonster
{
public:
	explicit CShambler(const CShambler& rhs);

private:
	explicit CShambler(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShambler();

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
	static CShambler* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override; //상태 변하면 텍스처 바꿔주는 함수
	void	Shambler_Take_Dmg(const _float& fTimeDelta);
private:
	enum MINIBOSSSTATE {
		MINIBOSS_IDLE,
		MINIBOSS_RUN,
		MINIBOSS_ATK,
		MINIBOSS_BASH,
		MINIBOSS_MAGIC,
		MINIBOSS_HIT,
		MINIBOSS_DEAD,
		MINIBOSS_END
	};
	enum SHAMBLERPHASE {SHAMBLER_NORMAL, SHAMBLER_HALFCUT, SHAMBLER_FULLCUT, SHAMBLER_END};

	MINIBOSSSTATE		m_eMiniBossState;
	MINIBOSSSTATE		m_ePreMiniBossState;

	SHAMBLERPHASE		m_ePhase;
	list<CIceBullet*>	m_lBullets;
	_int				m_iBulletCount;

	//추가 텍스처 컴포넌트
	Engine::CTexture* m_pTexCom_Bash;
	Engine::CTexture* m_pTexCom_Magic;

	Engine::CTexture* m_pHalfcutTexCom_Run;
	Engine::CTexture* m_pHalfcutTexCom_Atk;
	Engine::CTexture* m_pHalfcutTexCom_Magic;
	Engine::CTexture* m_pHalfcutTexCom_Hit;
	Engine::CTexture* m_pHalfcutTexCom_Dead;

	Engine::CTexture* m_pFullcutTexCom_Run;
	Engine::CTexture* m_pFullcutTexCom_Atk;
	Engine::CTexture* m_pFullcutTexCom_Hit;
	Engine::CTexture* m_pFullcutTexCom_Dead;

};

