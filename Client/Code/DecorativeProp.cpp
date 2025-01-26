#include "stdafx.h"
#include "..\Header\DecorativeProp.h"

#include "Export_System.h"
#include "Export_Utility.h"

CDecorativeProp::CDecorativeProp(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID)
	: CProp(pGraphicDev), m_eDecoID(eDecoID)
{
}

CDecorativeProp::CDecorativeProp(const CDecorativeProp& rhs)
	: CProp(rhs), m_eDecoID(rhs.m_eDecoID)
{
}

CDecorativeProp::~CDecorativeProp()
{
}

HRESULT CDecorativeProp::Ready_GameObject(_float fRange)
{
	//m_eDecoID = DECO_LAMP;
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(-3.f,2.4f,-1.f);
	m_pTransformCom->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
	m_vPrePos = m_pTransformCom->Get_Pos();

	if (m_eDecoID == DECO_LAMP)
	{
		m_pLight = CLight::Create(m_pGraphicDev, LIGHT_LAMP);
		m_pLight->Set_Light_Pos(m_pTransformCom->Get_Pos());
		m_pLight->Set_Light_Range(fRange);
	}

	return S_OK;
}

HRESULT CDecorativeProp::Ready_GameObject(D3DXVECTOR3 vPos, _float fRange)
{
	//m_eDecoID = DECO_LAMP;
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
	m_vPrePos = m_pTransformCom->Get_Pos();

	if (m_eDecoID == DECO_LAMP)
	{
		m_pLight = CLight::Create(m_pGraphicDev, LIGHT_LAMP);
		m_pLight->Set_Light_Pos(m_pTransformCom->Get_Pos());
		m_pLight->Set_Light_Range(fRange);
	}

	return S_OK;
}

Engine::_int CDecorativeProp::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CDecorativeProp::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);

}

void CDecorativeProp::Render_GameObject()
{
	_matrix   matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	Engine::Enable_Material(m_pGraphicDev);

	if (m_eDecoID == DECO_LAMP)
	{
		// 모든 전등 동시에 깜박임
		//if (m_fLightFrame >= 190.f)
		//{
		//   m_fLightFrame = 0.f;
		//}
		//else if (m_fLightFrame >= 180.f)
		//{
		//   Engine::Enable_Light(m_pGraphicDev, LIGHT_LAMP, FALSE);
		//}
	}

}


void CDecorativeProp::TexNamer()
{
	switch (m_eDecoID)
	{
	case DECO_CORPSE:
		m_szTexName = L"Proto_CorpseTexture";
		break;
	case DECO_LAMP:
		m_szTexName = L"Proto_LampTexture";
		break;
	case DECO_LIGHTPOST:
		m_szTexName = L"Proto_LightpostTexture";
		break;
	case DECO_SIREN:
		m_szTexName = L"Proto_LampTexture";
		break;
	default:
		break;
	}
}
HRESULT CDecorativeProp::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_szTexName.c_str(), pComponent});

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CDecorativeProp* CDecorativeProp::Create(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID, _float fRange)
{
	CDecorativeProp* pInstance = new CDecorativeProp(pGraphicDev, eDecoID);

	if (FAILED(pInstance->Ready_GameObject(fRange)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Prop Create Failed");
		return nullptr;
	}

	return pInstance;
}

CDecorativeProp* CDecorativeProp::Create(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID, D3DXVECTOR3 vPos, _float fRange)
{
	CDecorativeProp* pInstance = new CDecorativeProp(pGraphicDev, eDecoID);

	if (FAILED(pInstance->Ready_GameObject(vPos, fRange)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Prop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDecorativeProp::Free()
{
	__super::Free();
	if (m_pLight)
		Safe_Release(m_pLight);
}
