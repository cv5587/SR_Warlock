#include "stdafx.h"
#include "Cursor.h"

#include "Export_System.h"

CCursor::CCursor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUICamera(pGraphicDev), m_pShopUI(nullptr)
{
}

CCursor::CCursor(const CCursor& rhs)
	:CUICamera(rhs), m_pShopUI(rhs.m_pShopUI)
{
}

CCursor::~CCursor()
{
}

HRESULT CCursor::Ready_GameObject(CShopUICamera* _pShopUI)
{
	Engine::CUICamera::Ready_UICamera();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 가장 최상단에 출력
	m_iSortOrder = 1001;
	m_pTransformCom->m_vScale.x = 16.f;
	m_pTransformCom->m_vScale.y = 16.f;
	m_pTransformCom->Set_Pos(_vec3(0.f, 0.f, 0.f));
	m_pShopUI = _pShopUI;

	return S_OK;
}

_int CCursor::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_UI, this);

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);
	if (m_pShopUI->IsActive())
	{
		m_pTransformCom->m_vScale.x = 16.f;
		m_pTransformCom->m_vScale.y = 16.f;
		m_pTransformCom->Set_Pos(_vec3((mousePos.x + 16.f) - WINCX * 0.5f, -(mousePos.y + 16.f) + WINCY * 0.5f, 0.0f));
	}
	else
	{
		m_pTransformCom->m_vScale.x = 19.f;
		m_pTransformCom->m_vScale.y = 19.f;
		mousePos.x = WINCX * 0.5f;
		mousePos.y = WINCY * 0.5f;

		//테스트할때 주석처리
		SetCursorPos(mousePos.x, mousePos.y);
		m_pTransformCom->Set_Pos(_vec3(mousePos.x - WINCX * 0.5f, -mousePos.y + WINCY * 0.5f, 0.0f));
	}
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CCursor::LateUpdate_GameObject(const float& fTimeDelta)
{
	Engine::CUICamera::LateUpdate_GameObject(fTimeDelta);
}

void CCursor::Render_GameObject()
{
	Engine::CUICamera::Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_pShopUI->IsActive())
	{
		if (Get_DIMouseState(DIM_LB)) m_pTextureCom->Set_Texture(1);
		else m_pTextureCom->Set_Texture(0);
	}
	else
	{
		m_pTextureCom->Set_Texture(2);
	}

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CCursor::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Cursor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Cursor", pComponent });

	return S_OK;
}

CCursor* CCursor::Create(LPDIRECT3DDEVICE9 pGraphicDev, CShopUICamera* _pShopUI)
{
	CCursor* pInstance = new CCursor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(_pShopUI)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Cursor Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CCursor::Free()
{
	Engine::CUICamera::Free();
}