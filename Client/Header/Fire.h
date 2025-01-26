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
	void Detect_Player(const _float& fTimeDelta); //�÷��̾� �Ÿ� ����ؼ� Spike On/Off �Ǵ�, ���� �浹ó���� ��ü

protected:
	bool m_bSpike;
	float m_fAtkTimer;
	CPlayer* m_pPlayer;
	//TODO : ��Ʈ ������ ����

private:
	bool		m_bDead;
	bool		m_bInit;
	_float		m_fKillTimer;	//���� �ð� ��� �뵵 ����
	_float		m_fKillSecond;	//�� ������ŭ �ð� �̵��� ����


};

