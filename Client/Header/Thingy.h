#pragma once
#include "Monster.h"
#include "SpikeBullet.h"
#include "IceBullet.h"
class CThingy :
    public CMonster
{
public:
	explicit CThingy(const CThingy& rhs);

private:
	explicit CThingy(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThingy();

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
	static CThingy* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override; //상태 변하면 텍스처 바꿔주는 함수
	void	Thingy_Take_Dmg(const _float& fTimeDelta);
private:
	enum THINGYSTATE {
		THINGY_IDLE,
		THINGY_MORPH,
		THINGY_RUN,
		THINGY_ATK1,
		THINGY_ATK2,
		THINGY_MAGIC,
		THINGY_HIT,
		THINGY_DEAD,
		THINGY_END
	};

	THINGYSTATE		m_eThingyState;
	THINGYSTATE		m_ePreThingyState;

	list<CIceBullet*>	m_lBullets;
	_int				m_iBulletCount;

	//추가 텍스처 컴포넌트

	Engine::CTexture* m_pTexCom_Morph;
	Engine::CTexture* m_pTexCom_Atk2;
	Engine::CTexture* m_pTexCom_Magic;

};

