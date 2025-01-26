#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCalculator;

END

class CItem : public Engine::CGameObject
{
public:
	explicit CItem(const CItem& rhs);

private:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, ITEMTYPE eItemType, _int iNum);
	virtual ~CItem();

public:
	virtual HRESULT Ready_GameObject(_vec3 vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;
	virtual void	Action_On_Collision(CGameObject* pGameObject, float fTimeDelta) override;
	virtual void	Action_On_Collision(vector<CLine*> pvecLines) {}


	ITEMTYPE Get_ItemType() { return m_eItemtype; }
	_int Get_ItemNo() { return m_iNum; }
	_int Get_ItemValue() { return m_iValue; }

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;
	ITEMTYPE m_eItemtype;
	_int	 m_iNum;
	_int	 m_iValue;
	_float	 m_fFrame;

public:
	static CItem* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 vPos, ITEMTYPE eItemType, _int iNum = 0);
	static CItem* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 vPos);

private:
	virtual void Free() override;
};