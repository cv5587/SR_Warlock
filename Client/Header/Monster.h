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
	void						Take_Dmg(int iDmg) { if (this != nullptr) { m_iCurHealth -= iDmg; m_bHit = true; } }; //������ ����
	int							Get_Dmg() { return m_iDamage; }
	float						Get_HpRatio() { return (float)m_iCurHealth / (float)m_iMaxHealth; }

	void						Set_Detect_Range(_float detectRange) { m_fDtctRng = detectRange; }
protected:
	virtual  HRESULT			Add_Component();
	virtual void				Motion_Change();	//���� ���ϸ� �ؽ�ó �ٲ��ִ� �Լ�

	_float						DistanceToPlayer(CPlayer* pPlayer); // �÷��̾� ��ġ == ī�޶� Eye�Ÿ� ���

	//�� �� �Լ��� �������� ���
	void						Chase_Player(CPlayer* pPlayer, const _float& fTimeDelta); //�÷��̾� ����
	void						Knock_Back(CPlayer* pPlayer, //�˹� : �÷��̾� �ݴ� �������� �з���
										const _float& fSpeed, //�з����� �Ÿ�
										const _float& fTimeDelta);



protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom = nullptr;

protected:
	enum MONSTERSTATE	//�Ϲ� ���� ����
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
	wstring				m_szTexName;	//�ؽ�ó �̸�
	wstring				m_szName;	//�ؽ�ó �̸�

	_float		m_fAtkTimer;	//���� �ð� ��� ����
	_float		m_fStiffTimer;	//���� �ð� ��� ����
	_float		m_fStiffSecond; //���� �ð�

	//���� �⺻ ����
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

