#include "stdafx.h"
#include "..\Header\UIDynamicTex.h"

#include "Export_Utility.h"
#include "Export_System.h"

CUIDynamicTex::CUIDynamicTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr), m_fSizeX(1), m_fSizeY(1), m_fX(0), m_fY(0)
{
}

CUIDynamicTex::CUIDynamicTex(const CUIDynamicTex& rhs)
	: Engine::CGameObject(rhs),
	m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom),
	m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY),
	m_fX(rhs.m_fX), m_fY(rhs.m_fY)
{
}

CUIDynamicTex::~CUIDynamicTex()
{
}

HRESULT CUIDynamicTex::Ready_GameObject(wstring TextureTag, float fSizeX, float fSizeY, float fX, float fY, float fZ, bool bDirisRight, bool noLines)
{
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	m_fX = fX;
	m_fY = fY;


	FAILED_CHECK_RETURN(Add_Component(TextureTag, bDirisRight), E_FAIL);

	m_pTransformCom->m_vScale.x = m_fSizeX;
	m_pTransformCom->m_vScale.y = m_fSizeY;

	m_pTransformCom->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, fZ));

	D3DXMatrixIdentity(&m_ViewMatrix);

	/* 직교 투영을 수행하기 위한 행렬이다. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	m_noLines = noLines;
	if (m_noLines == false)
	{
		D3DXCreateLine(m_pGraphicDev, &m_pLine);
		m_pLine->SetWidth(2);
		m_pLine->SetAntialias(true);
		vertices[0] = { m_fX - fSizeX / 2 - 2 , m_fY - fSizeY / 2 - 2 };
		vertices[1] = { m_fX - fSizeX / 2 - 2,  m_fY + fSizeY / 2 + 2 };
		vertices[2] = { m_fX + fSizeX / 2 + 2,  m_fY + fSizeY / 2 + 2 };
		vertices[3] = { m_fX + fSizeX / 2 + 2,  m_fY - fSizeY / 2 - 2 };
		vertices[4] = { m_fX - fSizeX / 2 - 2 , m_fY - fSizeY / 2 - 2 };
	}

	return S_OK;
}

Engine::_int CUIDynamicTex::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CUIDynamicTex::LateUpdate_GameObject(const float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CUIDynamicTex::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_noLines == false)
	{
		m_pLine->Draw(vertices, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	m_pTextureCom->Set_Texture(m_texIdx);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CUIDynamicTex::Add_Component(wstring TextrueTag, bool bDirisRight)
{
	CComponent* pComponent = nullptr;

	if (bDirisRight)
	{
		pComponent = m_pBufferCom = dynamic_cast<CDynamicRcTex*>(Engine::Clone_Proto(L"Proto_DynamicRcTex1x1"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_DynamicRcTex", pComponent });
	}
	else
	{
		pComponent = m_pBufferCom = dynamic_cast<CDynamicRcTex*>(Engine::Clone_Proto(L"Proto_DynamicRcTex1x1_left"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_DynamicRcTex", pComponent });
	}


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(TextrueTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LogoTexture", pComponent });

	pComponent = m_pTransformCom_Clone = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CUIDynamicTex* CUIDynamicTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring TextureTag, float fSizeX, float fSizeY, float fX, float fY, float fZ, bool bDirisRight, bool noLines)
{
	CUIDynamicTex* pInstance = new CUIDynamicTex(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(TextureTag, fSizeX, fSizeY, fX, fY, fZ, bDirisRight, noLines)))
	{
		Safe_Release(pInstance);
		MSG_BOX("UITex Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUIDynamicTex::Free()
{
	__super::Free();
}
