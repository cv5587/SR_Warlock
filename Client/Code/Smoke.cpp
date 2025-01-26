#include "stdafx.h"
#include "..\Header\Smoke.h"
#include "Export_Utility.h"
#include <Monster.h>

CSmoke::CSmoke(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_fFrame(0.f)
{
}

CSmoke::CSmoke(const CSmoke& rhs)
	: CGameObject(rhs), m_fFrame(0.f)
{
}

CSmoke::~CSmoke()
{
}

HRESULT CSmoke::Ready_GameObject(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(vPos);
	return S_OK;
}

_int CSmoke::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrame += 90.f * fTimeDelta;
	if (m_fFrame * 2.f > 255.f) return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);
	m_pTransformCom->Update_Component(fTimeDelta);
	Compute_ViewZ(m_pTransformCom->Get_Pos());
	return S_OK;
}

void CSmoke::Render_GameObject()
{
	// 3Ãà ºôº¸µå
	_vec3 vPos;
	_matrix	matWorld, matView, matBill;
	vPos = m_pTransformCom_Clone->Get_Pos();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);
	D3DXMatrixInverse(&matView, NULL, &matView);
	matView._41 = vPos.x;
	matView._42 = vPos.y;
	matView._43 = vPos.z;
	m_pTransformCom->Set_WorldMatrix(&(matView));

	Engine::Enable_Material(m_pGraphicDev);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_int(255 - m_fFrame * 2.f), 255, 255, 255));

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CSmoke::Add_Component()
{
	CComponent* pComponent = nullptr;
	
	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Smoke"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Smoke", pComponent });

	return S_OK;
}

CSmoke* CSmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSmoke*	pInstance = DBG_NEW CSmoke(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Smoke Create Failed");
		return nullptr;
	}

	return pInstance;
}