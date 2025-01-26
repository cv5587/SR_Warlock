#pragma once

#include "Base.h"
#include "InteractiveProp.h"

class CExit : public CInteractiveProp
{
public:
	explicit CExit(const CExit& rhs);

private:
	explicit CExit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExit();

public:
	bool Is_Teleport_Pos() { return m_bTeleport; }


	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	static CExit* Create(LPDIRECT3DDEVICE9	pGraphicDev);

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
	void Scene_Change(); // �÷��̾� ��ġ �̵�
private:
	PORTALID m_ePortalID;
	_bool m_bWarp; //true�Ǹ� ���� �ö�
	_bool m_bTeleport;
	_bool m_bSound;

};

