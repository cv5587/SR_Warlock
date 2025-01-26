#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CSmoke :	public CGameObject
{
public:
	explicit CSmoke(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSmoke(const CSmoke& rhs);
	virtual ~CSmoke();

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

public:
	static CSmoke*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

};