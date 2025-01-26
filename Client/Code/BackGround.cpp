#include "stdafx.h"
#include "..\Header\BackGround.h"

#include "Export_Utility.h"
#include "Export_System.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev, float fSizeX, float fSizeY, float fX, float fY, float fZ)
	:Engine::CGameObject(pGraphicDev),m_fSizeX(fSizeX),m_fSizeY(fSizeY),m_fX(fX),m_fY(fY),m_fZ(fZ), m_PrintTextureIdx(0),m_iAlpha(255), m_fElipseTime(0.f)
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	: Engine::CGameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale.x = m_fSizeX;
	m_pTransformCom->m_vScale.y = m_fSizeY;

	m_pTransformCom->Set_Pos(_vec3(m_fX - WINCX * 0.5f, -m_fY + WINCY * 0.5f, m_fZ));

	D3DXMatrixIdentity(&m_ViewMatrix);

	/* 직교 투영을 수행하기 위한 행렬이다. */
	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	m_szPrintStr[0] = L"팀장";
	m_szPrintStr[1] = L"팀원0";
	m_szPrintStr[2] = L"팀원1";
	m_szPrintStr[3] = L"팀원2";


	return S_OK;
}

Engine::_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	m_fElipseTime += fTimeDelta;
	if (m_fElipseTime > 0.01f )
	{
		m_fElipseTime = 0.f;
		if (m_iAlpha > 0)
		{
			m_iAlpha--;
		}
		else
		{
			m_iAlpha = 255;

			m_PrintTextureIdx = (m_PrintTextureIdx + 1) % 4;
		}
	}



	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CBackGround::LateUpdate_GameObject(const _float& fTimeDelta)
{
	__super::LateUpdate_GameObject(fTimeDelta);
}

void CBackGround::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);



	for (int i = 0; i < 2; i++)
	{
		_matrix  matTmp = *m_pTransformCom->Get_WorldMatrix();

		matTmp.m[3][2] -= 0.1f * i;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

		if (i == 0)
		{
			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
			Engine::Render_Font(L"Font_Soo", m_szPrintStr[(m_PrintTextureIdx + i) % 4], &_vec2(200, 270 + 200 - m_iAlpha), D3DCOLOR_ARGB(m_iAlpha, 256, 256, 256));
		}
		else
		{
			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255 - m_iAlpha, 255, 255, 255));
			Engine::Render_Font(L"Font_Soo", m_szPrintStr[(m_PrintTextureIdx + i) % 4], &_vec2(200, 270 + 200 - (255 - m_iAlpha)), D3DCOLOR_ARGB(255 - m_iAlpha, 256, 256, 256));
		}

		m_pTextureCom[(m_PrintTextureIdx + i) % 4]->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

	}


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CBackGround::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex1x1"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	TCHAR szProtoName[128] = L"Proto_LogoTexture%d";

	for (int i = 0; i < 4; ++i)
	{

		TCHAR	szName[128] = L"";

		wsprintf(szName, szProtoName, i);

		pComponent = m_pTextureCom[i] = dynamic_cast<CTexture*>(Engine::Clone_Proto(szName));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ wstring(szName), pComponent });
	}


	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9	pGraphicDev, float fSizeX, float fSizeY, float fX, float fY, float fZ)
{
	CBackGround* pInstance = new CBackGround(pGraphicDev,fSizeX,fSizeY,fX,fY,fZ);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
}
