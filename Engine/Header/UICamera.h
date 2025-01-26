#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUICamera : public CGameObject
{
protected:
	explicit CUICamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUICamera();

public:
	virtual HRESULT Ready_UICamera();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(const float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	_matrix			m_matWorld, m_matView, m_matProj;
	//출력 순서 - 값이 작을수록 맨 위에 출력됨
	_int m_iSortOrder;

protected:
	virtual void	Free();
};

END