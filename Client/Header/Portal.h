#pragma once

#include "Base.h"
#include "InteractiveProp.h"

class CPortal : public CInteractiveProp
{
public:
	explicit CPortal(const CPortal& rhs);

private:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphicDev, PORTALID ePortalID);
	virtual ~CPortal();


public:
	static CPortal* Create(LPDIRECT3DDEVICE9	pGraphicDev, PORTALID ePortalID);


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;


private:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;
	virtual void Free()										 override;

	virtual  HRESULT	Add_Component();
	virtual void TexNamer() override;

private:
	void Detect_Player(); //�÷��̾� �Ÿ� ����ؼ� Warp On/Off �Ǵ�, ���� �浹ó���� ��ü
	void Warp_Player(); // �÷��̾� ��ġ �̵�
private:
	PORTALID m_ePortalID;
	_bool m_bWarp; //true�Ǹ� ���� �ö�
	_bool m_bSound;
};

