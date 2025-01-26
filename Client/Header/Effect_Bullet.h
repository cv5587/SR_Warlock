#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CEffect_Bullet :	public CGameObject
{
public:
	explicit CEffect_Bullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Bullet(const CEffect_Bullet& rhs);
	virtual ~CEffect_Bullet();

public:
	virtual HRESULT Ready_GameObject(_vec3 vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject()						 override;

protected:
	virtual HRESULT         Add_Component();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	_float m_fFrame;
	_int m_iTexCount, m_iTexStart, m_iTexNum;

public:
	static CEffect_Bullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

};