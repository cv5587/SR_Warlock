#pragma once
#include "Prop.h"
#include "Player.h"

BEGIN(Engine)

class CLight;

END


class CDecorativeProp : public CProp
{
public:
	explicit CDecorativeProp(const CDecorativeProp& rhs);

protected:
	explicit CDecorativeProp(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID);
	virtual ~CDecorativeProp();

	

public:
	HRESULT Ready_GameObject(_float fRange);
	virtual HRESULT Ready_GameObject(D3DXVECTOR3 vPos, _float fRange);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;


public:

	CLight* Get_Light_Ptr() { return m_pLight; }

public:
	static CDecorativeProp* Create(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID, _float fRange = -1.f);
	static CDecorativeProp* Create(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID, D3DXVECTOR3 vPos, _float fRange = -1.f);

protected:
	virtual void				Free() override;
	virtual  HRESULT			Add_Component() override;
	virtual void				TexNamer() override;

protected:
	DECOID	m_eDecoID;


	CLight*		m_pLight = nullptr;

};

