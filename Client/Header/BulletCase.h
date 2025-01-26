#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CBulletCase :	public Engine::CGameObject
{
public:
	explicit CBulletCase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBulletCase(const CBulletCase& rhs);
	virtual ~CBulletCase();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta)					 override;
	virtual void Render_GameObject()						 override;
	void Set_Pos(_vec3 vPos);
	void Set_Dir(_vec3 Dir);

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	_float m_fSpeed = 3.f;
	_float m_fPower = 1.5f;
	_vec3 m_vDir;
	_vec3 m_vUp;
	_vec3 m_vRight;
	_float m_fFrame;

public:
	static CBulletCase*		Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};