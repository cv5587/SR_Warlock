#pragma once

#include "Base.h"
#include "BreakableProp.h"

class CBox : public CBreakableProp
{
public:
	explicit CBox(const CBox& rhs);

private:
	explicit CBox(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID);
	explicit CBox(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID, _bool isFront, _float yAngle = 0);
	virtual ~CBox();
public:

	virtual void   Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void   Action_On_Collision(vector<CLine*> pvecLines) override;

public:
	virtual HRESULT Ready_GameObject()                   override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)                override;
	virtual void Render_GameObject()                   override;

private:
	_float m_fYAngle;
	bool m_bIsZBox;
	bool m_bisFront;

public:
	static CBox* Create(LPDIRECT3DDEVICE9   pGraphicDev, BOXID eBoxID);
	static CBox* Create(LPDIRECT3DDEVICE9   pGraphicDev, BOXID eBoxID, _bool isFront, _float yAngle);

protected:
	virtual void Free() override;
	virtual HRESULT         Add_Component();
	virtual void TexNamer() override;
};
