#include "Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vEye, _vec3 vAt, _vec3 vUp, _float fFov, _float fAspect, _float fNear, _float fFar)
	: CGameObject(pGraphicDev), m_vEye(vEye), m_vAt(vAt), m_vUp(vUp), m_fFov(fFov), m_fAspect(fAspect), m_fNear(fNear), m_fFar(fFar)
{

}

CCamera::CCamera(const CCamera& rhs)
	:CGameObject(rhs), m_fFov(rhs.m_fFov), m_fAspect(rhs.m_fAspect), m_fNear(rhs.m_fNear), m_fFar(rhs.m_fFar)
{
}

CCamera::~CCamera()
{
}

void Engine::CCamera::Free()
{
	CGameObject::Free();
}

HRESULT Engine::CCamera::Ready_GameObject()
{	
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//CPipeline::MakeViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	//CPipeline::MakeProjMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	
	return S_OK;
}

Engine::_int Engine::CCamera::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void Engine::CCamera::LateUpdate_GameObject(const float& fTimeDelta)
{
	//만약 여기서 처리할 것이 있으면 transformcom 먼저 업데이트 하고 코드짜기
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vEye, _vec3 vAt, _vec3 vUp, _float fFov, _float fAspect, _float fNear, _float fFar)
{
	CCamera* pInstance = DBG_NEW CCamera(pGraphicDev, vEye, vAt, vUp, fFov, fAspect, fNear, fFar);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}
	return pInstance;
}


