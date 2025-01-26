#include "stdafx.h"
#include "..\Header\Effect_Bullet.h"
#include "Export_Utility.h"
#include <Monster.h>

CEffect_Bullet::CEffect_Bullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_fFrame(0.f)
{
}

CEffect_Bullet::CEffect_Bullet(const CEffect_Bullet& rhs)
	: CGameObject(rhs), m_fFrame(0.f)
{
}

CEffect_Bullet::~CEffect_Bullet()
{
}

HRESULT CEffect_Bullet::Ready_GameObject(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_int iRand = rand() % 4 + 1;
	switch (iRand)
	{
	case 1:
		m_iTexStart = 0;
		m_iTexCount = 4;
		break;
	case 2:
		m_iTexStart = 4;
		m_iTexCount = 3;
		break;
	case 3:
		m_iTexStart = 7;
		m_iTexCount = 5;
		break;
	case 4:
		m_iTexStart = 12;
		m_iTexCount = 3;
		break;
	}
	m_pTransformCom->Set_Pos(vPos);
	return S_OK;
}

_int CEffect_Bullet::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += 90.f * fTimeDelta;
	m_iTexNum = m_iTexStart + (_int)(m_fFrame / 10.f);
	if (m_iTexNum >= m_iTexStart + m_iTexCount) return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);
	m_pTransformCom->Update_Component(fTimeDelta);
	return S_OK;
}

void CEffect_Bullet::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iTexNum);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CEffect_Bullet::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Effect_Bullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Effect_Bullet", pComponent });

	return S_OK;
}

CEffect_Bullet* CEffect_Bullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEffect_Bullet*	pInstance = DBG_NEW CEffect_Bullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Effect_Bullet Create Failed");
		return nullptr;
	}

	return pInstance;
}