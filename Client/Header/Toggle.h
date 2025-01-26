#pragma once
#include "Base.h"
#include "InteractiveProp.h"

class CToggle : public CInteractiveProp
{
public:
	explicit CToggle(const CToggle& rhs);
private:
	explicit CToggle(LPDIRECT3DDEVICE9 pGraphicDev, TOGGLEID eToggleID);
	virtual ~CToggle();


public:
	static CToggle* Create(LPDIRECT3DDEVICE9	pGraphicDev, TOGGLEID eTogID);
	static CToggle* Create(LPDIRECT3DDEVICE9	pGraphicDev, TOGGLEID eTogID, D3DXVECTOR3 vPos, float fYAngle);
	void	Set_ToggleOn() { m_bToggleOn = true; }


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;


private:
	virtual HRESULT Ready_GameObject()						 override;
	HRESULT			Ready_GameObject(D3DXVECTOR3 vPos, float fYAngle);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;
	virtual void Free()										 override;

	virtual  HRESULT	Add_Component()						override;
	virtual void		TexNamer()							override;
private:
	void Detect_Player();

private:
	bool	m_bToggleOn;
	TOGGLEID m_eToggleID;
	DOORID  m_eDoorID;
	bool	m_bIsZToggle;
};

//������ ������ ���� : 
// 1. �÷��̾ ���� ���� ����� ������(m_bUnlock == true)
// 2. �÷��̾ Ű �Է� �ݰ� �ȿ� �ִ�
// 3. �÷��̾ ��ȣ�ۿ� Ű�� ������

// ��� ������ ���� :
// 1. �÷��̾ ���� ���� ���踦 ������ �ִ�
// 2. �÷��̾ Ű �Է� �ݰ� �ȿ� �ִ�
// 3. �÷��̾ ��ȣ�ۿ� Ű�� ������

// ����� ������ ��� �Ǵ°�
// 1. ����� �ؽ�ó�� �ٲ�
// 2. ��� ID�� ���� ���� ���� ������ m_bUnlock = true
// 
//���ٿͼ��Ұ� : ����ġ ��� . �浹ó���� ��ó�� �ִ� ���� �� �ø�����
//Ű �԰� ����ġ �Ѿ� �� �� �־���
//TODO : �ִϸ��̼� ������