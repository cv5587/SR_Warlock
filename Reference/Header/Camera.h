#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :	public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vEye, _vec3 vAt, _vec3 vUp, _float fFov, _float fAspect, _float fNear, _float fFar);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject(const float& fTimeDelta);

public:
	//Layer의 Renderer에서 호출
	void Set_View_Space() { m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView); }
	void Set_Projection_Space() { m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj); }

	void Set_Eye(D3DXVECTOR3 vEye) { m_vEye = vEye; }
	void Set_At(D3DXVECTOR3 vAt) { m_vAt = vAt; }
	void Set_Up(D3DXVECTOR3 vUp) { m_vUp = vUp; }

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 m_vEye, _vec3 m_vAt, _vec3 m_vUp, _float m_fFov, _float m_fAspect, _float m_fNear, _float m_fFar);

protected:
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix			m_matView, m_matProj;

protected:
	virtual void	Free();
};

END

