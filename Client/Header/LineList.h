#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CLine;

END


class CLineList :
	public CGameObject
{
public:
	explicit CLineList(const CLineList& rhs);

private:
	explicit CLineList(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLineList();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)	 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

private:
	vector<Engine::CLine*> m_pvecLineGroupCom;
	vector<Engine::CTransform*> m_pvecTransformCom;

public:
	static CLineList* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual void Free() override;
};

